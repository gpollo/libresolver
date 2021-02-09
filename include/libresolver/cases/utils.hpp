#ifndef LIBRESOLVER_INCLUDE_CASES_UTILS_HPP
#define LIBRESOLVER_INCLUDE_CASES_UTILS_HPP

#include <utility>

#include <libresolver/instruction.hpp>
#include <libresolver/operand.hpp>
#include <libresolver/pattern/tree.hpp>
#include <libresolver/placeholder.hpp>
#include <libresolver/utils/registers.hpp>

namespace libresolver::cases::utils {

/* register wrappers */

using reg_pair = std::pair<placeholder::reg, libresolver::utils::registers::size>;

static constexpr auto R1  = reg_pair(placeholder::reg::REG_1, libresolver::utils::registers::size::QWORD);
static constexpr auto R1D = reg_pair(placeholder::reg::REG_1, libresolver::utils::registers::size::DWORD);
static constexpr auto R1W = reg_pair(placeholder::reg::REG_1, libresolver::utils::registers::size::WORD);
static constexpr auto R1B = reg_pair(placeholder::reg::REG_1, libresolver::utils::registers::size::BYTE);

static constexpr auto R2  = reg_pair(placeholder::reg::REG_2, libresolver::utils::registers::size::QWORD);
static constexpr auto R2D = reg_pair(placeholder::reg::REG_2, libresolver::utils::registers::size::DWORD);
static constexpr auto R2W = reg_pair(placeholder::reg::REG_2, libresolver::utils::registers::size::WORD);
static constexpr auto R2B = reg_pair(placeholder::reg::REG_2, libresolver::utils::registers::size::BYTE);

static constexpr auto R3  = reg_pair(placeholder::reg::REG_3, libresolver::utils::registers::size::QWORD);
static constexpr auto R3D = reg_pair(placeholder::reg::REG_3, libresolver::utils::registers::size::DWORD);
static constexpr auto R3W = reg_pair(placeholder::reg::REG_3, libresolver::utils::registers::size::WORD);
static constexpr auto R3B = reg_pair(placeholder::reg::REG_3, libresolver::utils::registers::size::BYTE);

static constexpr auto R4  = reg_pair(placeholder::reg::REG_4, libresolver::utils::registers::size::QWORD);
static constexpr auto R4D = reg_pair(placeholder::reg::REG_4, libresolver::utils::registers::size::DWORD);
static constexpr auto R4W = reg_pair(placeholder::reg::REG_4, libresolver::utils::registers::size::WORD);
static constexpr auto R4B = reg_pair(placeholder::reg::REG_4, libresolver::utils::registers::size::BYTE);

static constexpr auto R5  = reg_pair(placeholder::reg::REG_5, libresolver::utils::registers::size::QWORD);
static constexpr auto R5D = reg_pair(placeholder::reg::REG_5, libresolver::utils::registers::size::DWORD);
static constexpr auto R5W = reg_pair(placeholder::reg::REG_5, libresolver::utils::registers::size::WORD);
static constexpr auto R5B = reg_pair(placeholder::reg::REG_5, libresolver::utils::registers::size::BYTE);

static constexpr auto R6  = reg_pair(placeholder::reg::REG_6, libresolver::utils::registers::size::QWORD);
static constexpr auto R6D = reg_pair(placeholder::reg::REG_6, libresolver::utils::registers::size::DWORD);
static constexpr auto R6W = reg_pair(placeholder::reg::REG_6, libresolver::utils::registers::size::WORD);
static constexpr auto R6B = reg_pair(placeholder::reg::REG_6, libresolver::utils::registers::size::BYTE);

static constexpr auto R7  = reg_pair(placeholder::reg::REG_7, libresolver::utils::registers::size::QWORD);
static constexpr auto R7D = reg_pair(placeholder::reg::REG_7, libresolver::utils::registers::size::DWORD);
static constexpr auto R7W = reg_pair(placeholder::reg::REG_7, libresolver::utils::registers::size::WORD);
static constexpr auto R7B = reg_pair(placeholder::reg::REG_7, libresolver::utils::registers::size::BYTE);

static constexpr auto R8  = reg_pair(placeholder::reg::REG_8, libresolver::utils::registers::size::QWORD);
static constexpr auto R8D = reg_pair(placeholder::reg::REG_8, libresolver::utils::registers::size::DWORD);
static constexpr auto R8W = reg_pair(placeholder::reg::REG_8, libresolver::utils::registers::size::WORD);
static constexpr auto R8B = reg_pair(placeholder::reg::REG_8, libresolver::utils::registers::size::BYTE);

static constexpr auto R9  = reg_pair(placeholder::reg::REG_9, libresolver::utils::registers::size::QWORD);
static constexpr auto R9D = reg_pair(placeholder::reg::REG_9, libresolver::utils::registers::size::DWORD);
static constexpr auto R9W = reg_pair(placeholder::reg::REG_9, libresolver::utils::registers::size::WORD);
static constexpr auto R9B = reg_pair(placeholder::reg::REG_9, libresolver::utils::registers::size::BYTE);

static constexpr auto RIP = reg_pair(placeholder::reg::RIP, libresolver::utils::registers::size::QWORD);
static constexpr auto EIP = reg_pair(placeholder::reg::RIP, libresolver::utils::registers::size::DWORD);

/* constant wrappers */

using value = placeholder::value;

static constexpr auto K1  = placeholder::value::VALUE_1;
static constexpr auto K2  = placeholder::value::VALUE_2;
static constexpr auto K3  = placeholder::value::VALUE_3;
static constexpr auto K4  = placeholder::value::VALUE_4;
static constexpr auto K5  = placeholder::value::VALUE_5;
static constexpr auto K6  = placeholder::value::VALUE_6;
static constexpr auto K7  = placeholder::value::VALUE_7;
static constexpr auto K8  = placeholder::value::VALUE_8;
static constexpr auto K9  = placeholder::value::VALUE_9;
static constexpr auto K10 = placeholder::value::VALUE_10;

/* operand wrappers */

static inline operand::reg_ptr reg(const reg_pair& reg) {
    return operand::make_reg(reg.first, reg.second);
}

static inline operand::imm_ptr imm(placeholder::value value) {
    return operand::make_imm(value);
}

static inline operand::mem2_ptr mem2(placeholder::value disp, const reg_pair& base) {
    return operand::make_mem2(disp, base.first, base.second);
}

static inline operand::mem3_ptr mem3(placeholder::value disp, const reg_pair& index, placeholder::value scale) {
    return operand::make_mem3(disp, index.first, index.second, scale);
}

static inline operand::mem4_ptr mem4(placeholder::value disp, const reg_pair& base, const reg_pair& index,
                                     placeholder::value scale) {
    return operand::make_mem4(disp, base.first, base.second, index.first, index.second, scale);
}

/* action wrappers */

enum class action_type {
    TRACK_REG,
    IGNORE_REG,
    TRACK_INSN,
    IGNORE_INSN,
};

struct action {
    action_type type_;
    placeholder::reg reg_;
    x86_insn insn_;

    static inline constexpr action track(placeholder::reg reg) {
        return {.type_ = action_type::TRACK_REG, .reg_ = reg};
    }

    static inline constexpr action ignore(placeholder::reg reg) {
        return {.type_ = action_type::IGNORE_REG, .reg_ = reg};
    }

    static inline constexpr action track(x86_insn insn) {
        return {.type_ = action_type::TRACK_INSN, .insn_ = insn};
    }

    static inline constexpr action ignore(x86_insn insn) {
        return {.type_ = action_type::IGNORE_INSN, .insn_ = insn};
    }
};

static constexpr auto TRACK_R1 = action::track(placeholder::reg::REG_1);
static constexpr auto TRACK_R2 = action::track(placeholder::reg::REG_2);
static constexpr auto TRACK_R3 = action::track(placeholder::reg::REG_3);
static constexpr auto TRACK_R4 = action::track(placeholder::reg::REG_4);
static constexpr auto TRACK_R5 = action::track(placeholder::reg::REG_5);
static constexpr auto TRACK_R6 = action::track(placeholder::reg::REG_6);
static constexpr auto TRACK_R7 = action::track(placeholder::reg::REG_7);
static constexpr auto TRACK_R8 = action::track(placeholder::reg::REG_8);
static constexpr auto TRACK_R9 = action::track(placeholder::reg::REG_9);

static constexpr auto IGNORE_R1 = action::ignore(placeholder::reg::REG_1);
static constexpr auto IGNORE_R2 = action::ignore(placeholder::reg::REG_2);
static constexpr auto IGNORE_R3 = action::ignore(placeholder::reg::REG_3);
static constexpr auto IGNORE_R4 = action::ignore(placeholder::reg::REG_4);
static constexpr auto IGNORE_R5 = action::ignore(placeholder::reg::REG_5);
static constexpr auto IGNORE_R6 = action::ignore(placeholder::reg::REG_6);
static constexpr auto IGNORE_R7 = action::ignore(placeholder::reg::REG_7);
static constexpr auto IGNORE_R8 = action::ignore(placeholder::reg::REG_8);
static constexpr auto IGNORE_R9 = action::ignore(placeholder::reg::REG_9);

static constexpr auto TRACK_INS_CMP = action::track(x86_insn::X86_INS_CMP);
static constexpr auto TRACK_INS_JA  = action::track(x86_insn::X86_INS_JA);
static constexpr auto TRACK_INS_JB  = action::track(x86_insn::X86_INS_JB);

static constexpr auto IGNORE_INS_CMP = action::ignore(x86_insn::X86_INS_CMP);
static constexpr auto IGNORE_INS_JA  = action::ignore(x86_insn::X86_INS_JA);
static constexpr auto IGNORE_INS_JB  = action::ignore(x86_insn::X86_INS_JB);

static pattern::actions create_actions(const std::vector<action>& actions) {
    pattern::actions tree_actions;

    for (auto action : actions) {
        switch (action.type_) {
        case action_type::TRACK_REG:
            tree_actions.track_regs_.insert(action.reg_);
            break;
        case action_type::IGNORE_REG:
            tree_actions.ignore_regs_.insert(action.reg_);
            break;
        case action_type::TRACK_INSN:
            tree_actions.track_insns_.insert(action.insn_);
            break;
        case action_type::IGNORE_INSN:
            tree_actions.ignore_insns_.insert(action.insn_);
            break;
        }
    }

    return tree_actions;
}

/* instruction wrappers */

template <typename Insn, typename... Actions>
pattern::tree::pattern_node create_no_op_insn(const std::vector<action>& actions = {}) {
    return {Insn(), create_actions(actions)};
}

template <typename Insn, typename... Actions>
pattern::tree::pattern_node create_one_op_insn(operand::base_ptr op, const std::vector<action>& actions = {}) {
    return {Insn(op), create_actions(actions)};
}

template <typename Insn, typename... Actions>
pattern::tree::pattern_node create_two_ops_insn(operand::base_ptr op1, operand::base_ptr op2,
                                                const std::vector<action>& actions = {}) {
    return {Insn(op1, op2), create_actions(actions)};
}

static const auto add    = create_two_ops_insn<instruction::add>;
static const auto sub    = create_two_ops_insn<instruction::sub>;
static const auto cmp    = create_two_ops_insn<instruction::cmp>;
static const auto cltq   = create_no_op_insn<instruction::cltq>;
static const auto ja     = create_one_op_insn<instruction::ja>;
static const auto jb     = create_one_op_insn<instruction::jb>;
static const auto jmp    = create_one_op_insn<instruction::jmp>;
static const auto lea    = create_two_ops_insn<instruction::lea>;
static const auto mov    = create_two_ops_insn<instruction::mov>;
static const auto movzbl = create_two_ops_insn<instruction::movzbl>;
static const auto movsbl = create_two_ops_insn<instruction::movsbl>;
static const auto movslq = create_two_ops_insn<instruction::movslq>;

} /* namespace libresolver::cases::utils */

#endif /* LIBRESOLVER_INCLUDE_CASES_UTILS_HPP */