#include <libresolver/pattern/actions.hpp>

namespace libresolver::pattern {

bool actions::operator==(const actions& other) const {
    return (track_regs_ == other.track_regs_) && (ignore_regs_ == other.ignore_regs_) &&
           (track_insns_ == other.track_insns_) && (ignore_insns_ == other.ignore_insns_);
}

bool actions::operator!=(const actions& other) const {
    return !(*this == other);
}

void actions::merge(const actions& other) {
    track_regs_.insert(other.track_regs_.begin(), other.track_regs_.end());
    ignore_regs_.insert(other.ignore_regs_.begin(), other.ignore_regs_.end());
    track_insns_.insert(other.track_insns_.begin(), other.track_insns_.end());
    ignore_insns_.insert(other.ignore_insns_.begin(), other.ignore_insns_.end());
}

}  // namespace libresolver::pattern

namespace std {

string to_string(x86_insn insn) {
    switch (insn) {
    case x86_insn::X86_INS_ADD:
        return "X86_INS_ADD";
    case x86_insn::X86_INS_CMP:
        return "X86_INS_CMP";
    case x86_insn::X86_INS_JA:
        return "X86_INS_JA";
    case x86_insn::X86_INS_JMP:
        return "X86_INS_JMP";
    case x86_insn::X86_INS_LEA:
        return "X86_INS_LEA";
    case x86_insn::X86_INS_MOV:
        return "X86_INS_MOV";
    case x86_insn::X86_INS_MOVSXD:
        return "X86_INS_MOVSXD";
    default:
        return "UNKNOWN";
    }
}

string to_string(const libresolver::pattern::actions& actions) {
    string str = "";

    if (actions.track_regs_.size() != 0 || actions.track_insns_.size() != 0) {
        str.append(" track={");

        for (auto reg : actions.track_regs_) {
            str.append(to_string(reg));
            str.append(" ");
        }

        for (auto insn : actions.track_insns_) {
            str.append(to_string_insn(insn));
            str.append(" ");
        }

        str.replace(str.length() - 1, str.length(), "}");
    }

    if (actions.ignore_regs_.size() != 0 || actions.ignore_insns_.size() != 0) {
        str.append(" ignore={");

        for (auto reg : actions.ignore_regs_) {
            str.append(to_string(reg));
            str.append(" ");
        }

        for (auto insn : actions.ignore_insns_) {
            str.append(to_string_insn(insn));
            str.append(" ");
        }

        str.replace(str.length() - 1, str.length(), "}");
    }

    return str;
}

} /* namespace std */