#ifndef LIBRESOLVER_INCLUDE_INSTRUCTION_HPP
#define LIBRESOLVER_INCLUDE_INSTRUCTION_HPP

#include <string>
#include <vector>

#include <capstone/capstone.h>
#include <libresolver/operand.hpp>
#include <libresolver/utils/hash.hpp>
#include <libresolver/utils/memory.hpp>

namespace libresolver::instruction {

class base {
   public:
    base(x86_insn insn);
    const x86_insn get_insn() const;
    const std::vector<operand::base_ptr>& get_operands() const;
    const bool operator==(const base& other) const;

   protected:
    x86_insn insn_;
    std::vector<operand::base_ptr> operands_;

    friend std::hash<base>;
};

template <x86_insn Insn>
class zero_op : public base {
   public:
    static constexpr x86_insn INSN = Insn;

    zero_op() : base(Insn) {}
};

template <x86_insn Insn>
class one_op : public base {
   public:
    static constexpr x86_insn INSN = Insn;

    template <typename T>
    one_op(std::shared_ptr<T> op) : base(Insn) {
        operands_.push_back(op);
    }
};

template <x86_insn Insn>
class two_ops : public base {
   public:
    static constexpr x86_insn INSN = Insn;

    template <typename T1, typename T2>
    two_ops(std::shared_ptr<T1> op1, std::shared_ptr<T2> op2) : base(Insn) {
        operands_.push_back(op1);
        operands_.push_back(op2);
    }
};

using add    = instruction::two_ops<x86_insn::X86_INS_ADD>;
using sub    = instruction::two_ops<x86_insn::X86_INS_SUB>;
using call   = instruction::one_op<x86_insn::X86_INS_CALL>;
using cmp    = instruction::two_ops<x86_insn::X86_INS_CMP>;
using cltq   = instruction::zero_op<x86_insn::X86_INS_CDQE>;
using ja     = instruction::one_op<x86_insn::X86_INS_JA>;
using jb     = instruction::one_op<x86_insn::X86_INS_JB>;
using jmp    = instruction::one_op<x86_insn::X86_INS_JMP>;
using lea    = instruction::two_ops<x86_insn::X86_INS_LEA>;
using mov    = instruction::two_ops<x86_insn::X86_INS_MOV>;
using movzbl = instruction::two_ops<x86_insn::X86_INS_MOVZX>;
using movsbl = instruction::two_ops<x86_insn::X86_INS_MOVSX>;
using movslq = instruction::two_ops<x86_insn::X86_INS_MOVSXD>;

} /* namespace libresolver::instruction */

namespace std {

template <>
struct hash<libresolver::instruction::base> {
    size_t operator()(const libresolver::instruction::base& insn) const {
        size_t seed = std::hash<x86_insn>{}(insn.insn_);
        return libresolver::utils::hash::combine(seed, insn.operands_.begin(), insn.operands_.end());
    }
};

string to_string_insn(x86_insn insn);
string to_string(const libresolver::instruction::base& insn);

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_INSTRUCTION_HPP */