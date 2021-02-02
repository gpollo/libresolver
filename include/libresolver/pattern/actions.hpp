#ifndef LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP

#include <unordered_set>

#include <capstone/capstone.h>
#include <libresolver/instruction.hpp>
#include <libresolver/placeholder.hpp>

namespace libresolver::pattern {

struct actions {
    std::unordered_set<placeholder::reg> track_regs_  = {};
    std::unordered_set<placeholder::reg> ignore_regs_ = {};
    std::unordered_set<x86_insn> track_insns_         = {};
    std::unordered_set<x86_insn> ignore_insns_        = {};

    bool operator==(const actions& other) const;
    bool operator!=(const actions& other) const;
    void merge(const actions& other);
};

} /* namespace libresolver::pattern */

namespace std {

string to_string(x86_insn insn);
string to_string(const libresolver::pattern::actions& actions);

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_PATTERN_ACTIONS_HPP */