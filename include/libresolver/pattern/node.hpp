#ifndef LIBRESOLVER_INCLUDE_PATTERN_NODE_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_NODE_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <libresolver/instruction.hpp>
#include <libresolver/log.hpp>
#include <libresolver/pattern/actions.hpp>
#include <libresolver/pattern/match.hpp>
#include <libresolver/placeholder.hpp>
#include <libresolver/utils/hash.hpp>

namespace libresolver::pattern {

class node {
   public:
    node() : actions_() {}
    node(const actions& actions) : actions_(actions) {}

    const actions& get_actions() const {
        return actions_;
    }

    const std::vector<std::shared_ptr<match>>& get_matches() const {
        return matches_;
    }

    bool insert(const instruction::base& instruction, const actions& actions) {
        if (matches_.size() != 0) {
            ERR("node already contains matches");
            return false;
        }

        if (childs_->contains(instruction)) {
            const auto& current_node = childs_->at(instruction);
            if (current_node.get_actions() != actions) {
                ERR("current node actions are different");
                ERR("instruction: " << std::to_string(instruction));
                ERR("   old actions: " << std::to_string(current_node.get_actions()));
                ERR("   new actions: " << std::to_string(actions));
                return false;
            }
        } else {
            childs_->insert({instruction, node(actions)});
        }

        return true;
    }

    bool insert(const std::shared_ptr<match> match) {
        if (childs_->size() != 0) {
            ERR("node already contains children");
            return false;
        }

        matches_.push_back(match);

        return true;
    }

    std::optional<std::reference_wrapper<node>> get(const instruction::base& instruction) const {
        if (!childs_->contains(instruction)) {
            return {};
        }

        return childs_->at(instruction);
    }

    std::vector<instruction::base> get_child_instructions() const {
        std::vector<instruction::base> instructions;

        for (auto it = childs_->cbegin(); it != childs_->cend(); it++) {
            instructions.push_back(it->first);
        }

        return instructions;
    }

   private:
    const actions actions_;
    std::unique_ptr<std::unordered_map<instruction::base, node>> childs_ =
        std::make_unique<std::unordered_map<instruction::base, node>>();
    std::vector<std::shared_ptr<match>> matches_ = {};
};

} /* namespace libresolver::pattern */

#endif /* LIBRESOLVER_INCLUDE_PATTERN_NODE_HPP */