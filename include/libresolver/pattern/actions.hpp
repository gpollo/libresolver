#ifndef LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP

#include <capstone/capstone.h>
#include <libresolver/instruction.hpp>
#include <libresolver/placeholder.hpp>

namespace libresolver::pattern {

struct actions {
    std::vector<placeholder::reg> track_regs_  = {};
    std::vector<placeholder::reg> ignore_regs_ = {};
    std::vector<x86_insn> track_insns_         = {};
    std::vector<x86_insn> ignore_insns_        = {};

    bool operator==(const actions& other) const {
        return (track_regs_ == other.track_regs_) && (ignore_regs_ == other.ignore_regs_) &&
               (track_insns_ == other.track_insns_) && (ignore_insns_ == other.ignore_insns_);
    }

    bool operator!=(const actions& other) const {
        return !(*this == other);
    }
};

} /* namespace libresolver::pattern */

namespace std {

static inline string to_string(x86_insn insn) {
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

static inline string to_string(const libresolver::pattern::actions& actions) {
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

#endif /* LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP */