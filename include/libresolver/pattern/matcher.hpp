#ifndef LIBRESOLVER_INCLUDE_PATTERN_MATCHER_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_MATCHER_HPP

#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include <libresolver/context.hpp>
#include <libresolver/pattern/match.hpp>
#include <libresolver/pattern/tree.hpp>
#include <libresolver/watch_list.hpp>

namespace libresolver::pattern {

class matcher {
   public:
    matcher(const tree& tree, bool using_intel_syntax = false) : tree_(tree) {
        context_.using_intel_syntax_ = using_intel_syntax;
    };

    const context& get_context() const {
        return context_;
    }

    const std::vector<std::shared_ptr<match>> match_instructions(
        const std::vector<std::reference_wrapper<cs_insn>>& insns) {
        context_.reset();
        watch_list_.reset();
        watch_list_.add_instruction(x86_insn::X86_INS_JMP);
        bool first_matched = false;

        node* current_node = nullptr;
        for (auto& insn : insns) {
            if (!watch_list_.matches(insn)) {
                continue;
            }

            if (!first_matched) {
                watch_list_.remove_instruction(x86_insn::X86_INS_JMP);
                first_matched = true;
            }

            auto instruction_opt = context_.build_instruction(insn);
            if (!instruction_opt.has_value()) {
                std::cerr << "[pattern::matcher::match_instructions] failed to build instruction" << std::endl;
                return {};
            }
            auto instruction = instruction_opt.value();

            std::cout << "processing " << std::to_string(instruction) << std::endl;

            std::optional<std::reference_wrapper<node>> node_opt;
            if (current_node == nullptr) {
                node_opt = tree_.get_root().get(instruction);
            } else {
                node_opt = current_node->get(instruction);
            }

            if (!node_opt.has_value()) {
                std::cerr << "[pattern::matcher::match_instructions] instruction didn't match" << std::endl;
                std::cerr << "[pattern::matcher::match_instructions] -> " << std::to_string(instruction) << std::endl;
                return {};
            }
            current_node = &node_opt.value().get();

            context_.validate_last_instruction();
            update_watch_list(current_node->get_actions());
        }

        return current_node->get_matches();
    }

   protected:
    const tree& tree_;
    context context_;
    watch_list watch_list_;

    void update_watch_list(const actions& actions) {
        for (auto placeholder_reg : actions.track_regs_) {
            auto reg_opt = context_.get(placeholder_reg);
            if (!reg_opt.has_value()) {
                std::cerr << "[pattern::matcher::update_watch_list] expected placeholder `"
                          << std::to_string(placeholder_reg) << "` to have been set" << std::endl;
                continue;
            }

            watch_list_.add_register(reg_opt.value());
        }

        for (auto placeholder_reg : actions.ignore_regs_) {
            auto reg_opt = context_.get(placeholder_reg);
            if (!reg_opt.has_value()) {
                std::cerr << "[pattern::matcher::update_watch_list] expected placeholder `"
                          << std::to_string(placeholder_reg) << "` to have been set" << std::endl;
                continue;
            }

            watch_list_.remove_register(reg_opt.value());
        }

        for (auto insn : actions.track_insns_) {
            watch_list_.add_instruction(insn);
        }

        for (auto insn : actions.ignore_insns_) {
            watch_list_.remove_instruction(insn);
        }
    }
};

} /* namespace libresolver::pattern */

#endif /* LIBRESOLVER_INCLUDE_PATTERN_MATCHER_HPP */
