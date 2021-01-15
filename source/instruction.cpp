#include <libresolver/instruction.hpp>

namespace libresolver::instruction {

base::base(x86_insn insn) : insn_(insn) {}

const x86_insn base::get_insn() const {
    return insn_;
}

const std::vector<operand::base_ptr>& base::get_operands() const {
    return operands_;
}

const bool base::operator==(const base& other) const {
    return (insn_ == other.insn_) && utils::memory::compare_shared_ptr_vector(operands_, other.operands_);
}

} /* namespace libresolver::instruction */

namespace std {

string to_string_insn(x86_insn insn) {
    switch (insn) {
    case x86_insn::X86_INS_ADD:
        return "add";
    case x86_insn::X86_INS_CMP:
        return "cmp";
    case x86_insn::X86_INS_CDQE:
        return "cltq";
    case x86_insn::X86_INS_JA:
        return "ja";
    case x86_insn::X86_INS_JB:
        return "jb";
    case x86_insn::X86_INS_JMP:
        return "jmp";
    case x86_insn::X86_INS_LEA:
        return "lea";
    case x86_insn::X86_INS_MOV:
        return "mov";
    case x86_insn::X86_INS_MOVZX:
        return "movzx";
    case x86_insn::X86_INS_MOVSXD:
        return "movsxd";
    default:
        break;
    }

    return "unknown-instruction";
}

string to_string(const libresolver::instruction::base& insn) {
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