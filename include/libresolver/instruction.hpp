#ifndef LIBRESOLVER_INCLUDE_INSTRUCTION_HPP
#define LIBRESOLVER_INCLUDE_INSTRUCTION_HPP

#include <vector>

#include <capstone/capstone.h>
#include <libresolver/operand.hpp>
#include <libresolver/utils/hash.hpp>
#include <libresolver/utils/memory.hpp>

namespace libresolver::instruction {

class base {
   public:
    base(x86_insn insn) : insn_(insn) {}

    const x86_insn get_insn() const {
        return insn_;
    }

    const std::vector<operand::base_ptr>& get_operands() const {
        return operands_;
    }

    const bool operator==(const base& other) const {
        return (insn_ == other.insn_) && utils::memory::compare_shared_ptr_vector(operands_, other.operands_);
    }

   protected:
    const x86_insn insn_;
    std::vector<operand::base_ptr> operands_;

    friend std::hash<base>;
};

template <x86_insn Insn>
class one_op : public base {
   public:
    template <typename T>
    one_op(std::shared_ptr<T> op) : base(Insn) {
        operands_.push_back(op);
    }
};

template <x86_insn Insn>
class two_ops : public base {
   public:
    template <typename T1, typename T2>
    two_ops(std::shared_ptr<T1> op1, std::shared_ptr<T2> op2) : base(Insn) {
        operands_.push_back(op1);
        operands_.push_back(op2);
    }
};

using add    = instruction::two_ops<x86_insn::X86_INS_ADD>;
using cmp    = instruction::two_ops<x86_insn::X86_INS_CMP>;
using ja     = instruction::one_op<x86_insn::X86_INS_JA>;
using jmp    = instruction::one_op<x86_insn::X86_INS_JMP>;
using lea    = instruction::two_ops<x86_insn::X86_INS_LEA>;
using mov    = instruction::two_ops<x86_insn::X86_INS_MOV>;
using movsxd = instruction::two_ops<x86_insn::X86_INS_MOVSXD>;

} /* namespace libresolver::instruction */

namespace std {

template <>
struct hash<libresolver::instruction::base> {
    std::size_t operator()(const libresolver::instruction::base& insn) const {
        size_t seed = std::hash<x86_insn>{}(insn.insn_);
        return libresolver::utils::hash::combine(seed, insn.operands_.begin(), insn.operands_.end());
    }
};

static inline string to_string_insn(x86_insn insn) {
    switch (insn) {
    case x86_insn::X86_INS_ADD:
        return "add";
    case x86_insn::X86_INS_CMP:
        return "cmp";
    case x86_insn::X86_INS_JA:
        return "ja";
    case x86_insn::X86_INS_JMP:
        return "jmp";
    case x86_insn::X86_INS_LEA:
        return "lea";
    case x86_insn::X86_INS_MOV:
        return "mov";
    case x86_insn::X86_INS_MOVSXD:
        return "movsxd";
    default:
        break;
    }

    return "unknown-instruction";
}

static inline string to_string(const libresolver::instruction::base& insn) {
    auto str      = to_string_insn(insn.get_insn()) + " ";
    auto operands = insn.get_operands();

    for (unsigned int i = 0; i < operands.size(); i++) {
        str.append(to_string(*operands[i]));

        if (i < operands.size() - 1) {
            str.append(", ");
        }
    }

    return str;
}

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_INSTRUCTION_HPP */