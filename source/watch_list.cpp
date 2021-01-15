#include <iostream>

#include <libresolver/log.hpp>
#include <libresolver/utils/registers.hpp>
#include <libresolver/watch_list.hpp>

namespace libresolver {

void watch_list::reset() {
    registers_.clear();
    instructions_.clear();
}

bool watch_list::matches(cs_insn& insn) const {
    if (instructions_.contains(static_cast<x86_insn>(insn.id))) {
        return true;
    }

    auto regs_opt = utils::registers::written_of(insn);
    if (!regs_opt.has_value()) {
        return false;
    }

    for (auto reg : regs_opt.value()) {
        if (registers_.contains(static_cast<x86_reg>(reg))) {
            return true;
        }
    }

    return false;
}

bool watch_list::add_register(x86_reg reg) {
    auto regs_opt = utils::registers::group_of(reg);
    if (!regs_opt.has_value()) {
        ERR("failed to obtain register group");
        return false;
    }

    for (auto reg_value : regs_opt.value()) {
        registers_.insert(reg_value);
    }

    return true;
}

bool watch_list::remove_register(x86_reg reg) {
    auto regs_opt = utils::registers::group_of(reg);
    if (!regs_opt.has_value()) {
        ERR("failed to obtain register group");
        return false;
    }

    for (auto reg_value : regs_opt.value()) {
        registers_.erase(reg_value);
    }

    return true;
}

void watch_list::add_instruction(x86_insn reg) {
    instructions_.insert(reg);
}

void watch_list::remove_instruction(x86_insn reg) {
    instructions_.erase(reg);
}

} /* namespace libresolver */