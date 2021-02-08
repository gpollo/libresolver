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
    case libresolver::instruction::add::INSN:
        return "add";
    case libresolver::instruction::sub::INSN:
        return "sub";
    case libresolver::instruction::call::INSN:
        return "call";
    case libresolver::instruction::cmp::INSN:
        return "cmp";
    case libresolver::instruction::cltq::INSN:
        return "cltq";
    case libresolver::instruction::ja::INSN:
        return "ja";
    case libresolver::instruction::jb::INSN:
        return "jb";
    case libresolver::instruction::jmp::INSN:
        return "jmp";
    case libresolver::instruction::lea::INSN:
        return "lea";
    case libresolver::instruction::mov::INSN:
        return "mov";
    case libresolver::instruction::movzbl::INSN:
        return "movzbl";
    case libresolver::instruction::movsbl::INSN:
        return "movsbl";
    case libresolver::instruction::movslq::INSN:
        return "movslq";
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