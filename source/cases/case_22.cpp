#include <libresolver/cases/case_22.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

namespace libresolver::cases {

using libresolver::placeholder::reg;
using libresolver::placeholder::value;
using libresolver::utils::registers::size;

case_22::case_22(arch arch) : case_base("case-22", arch) {}

pattern::tree::pattern case_22::get_pattern() const {
    /*
     * Under 32-bit systems, the %r2 register contains a pointer
     * into the global offset table (GOT). This pointer is obtained
     * with a call to a function like xx.get_pc_thunk.xx at the start
     * of a function.
     *
     * Under 64-bit systems, the %r2 register is resolved by a LEA
     * address relative to current instruction pointer.
     *
     * [32] call   k8
     * [32] add    $k7,%r2
     * [64] lea    k7(%rip),%r2
     *      cmpl   $k5,k6(%r4)
     *      ja     k4
     *      mov    k3(%r4),%r3
     * [32] mov    k1(%r2,%r3,k2),%r1
     * [64] movslq k1(%r2,%r3,k2),%r1
     *      add    %r2,%r1
     *      jmp    *%r1
     *
     * where k3 == k6;
     */

    utils::registers::size reg_size;
    switch (arch_) {
    case arch::X86_32:
        reg_size = size::DWORD;
        break;
    case arch::X86_64:
        reg_size = size::QWORD;
        break;
    default:
        return {};
    }

    pattern::tree::pattern pattern = {
        /* clang-format off */
        {
            instruction::jmp(
                operand::make_reg(reg::REG_1, reg_size)
            ), {
                .track_regs_ = {reg::REG_1},
            }
        }, {
            instruction::add(
                operand::make_reg(reg::REG_2, reg_size),
                operand::make_reg(reg::REG_1, reg_size)
            ), {
                .track_regs_ = {reg::REG_2},
            }
        }
        /* clang-format on */
    };

    if (arch_ == arch::X86_32) {
        pattern.push_back(
            /* clang-format off */
            {
                instruction::mov(
                    operand::make_mem4(value::VALUE_1, reg::REG_2, reg_size, reg::REG_3, reg_size, value::VALUE_2),
                    operand::make_reg(reg::REG_1, reg_size)
                ), {
                    .track_regs_ = {reg::REG_3},
                    .ignore_regs_ = {reg::REG_1},
                }    
            } /* clang-format on */
        );
    } else {
        pattern.push_back(
            /* clang-format off */
            {
                instruction::movslq(
                    operand::make_mem4(value::VALUE_1, reg::REG_2, reg_size, reg::REG_3, reg_size, value::VALUE_2),
                    operand::make_reg(reg::REG_1, reg_size)
                ), {
                    .track_regs_ = {reg::REG_3},
                    .ignore_regs_ = {reg::REG_1},
                }    
            } /* clang-format on */
        );
    }

    pattern.insert(
        /* clang-format off */
        pattern.end(), {
        {
            instruction::mov(
                operand::make_mem2(value::VALUE_3, reg::REG_4, reg_size),
                operand::make_reg(reg::REG_3, reg_size)
            ), {
                .track_regs_ = {reg::REG_4},
                .ignore_regs_ = {reg::REG_3},
                .track_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::ja(
                operand::make_imm(value::VALUE_4)
            ), {
                .track_insns_ = {X86_INS_CMP},
                .ignore_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::cmp(
                operand::make_imm(value::VALUE_5),
                operand::make_mem2(value::VALUE_6, reg::REG_4, reg_size)
            ), {
                .ignore_regs_ = {reg::REG_4},
                .ignore_insns_ = {X86_INS_CMP},
            }
        }                      
    }); /* clang-format on */

    if (arch_ == arch::X86_32) {
        pattern.insert(
            /* clang-format off */
            pattern.end(), {
            {
                instruction::add(
                    operand::make_imm(value::VALUE_7),
                    operand::make_reg(reg::REG_2, reg_size)
                ), {
                    .track_insns_ = {X86_INS_CALL},
                }
            }, {
                instruction::call(
                    operand::make_imm(value::VALUE_8)
                ), {
                    .ignore_regs_ = {reg::REG_2},
                    .ignore_insns_ = {X86_INS_CALL},
                }
            }
        }); /* clang-format on */
    } else {
        pattern.insert(
            /* clang-format off */
            pattern.end(), {
            {
                instruction::lea(
                    operand::make_mem2(value::VALUE_7, reg::RIP, reg_size),
                    operand::make_reg(reg::REG_2, reg_size)
                ), {
                    .ignore_regs_ = {reg::REG_2},
                }
            }
        }); /* clang-format on */
    }

    return pattern;
}

std::unordered_set<uint64_t> case_22::evaluate(const context& context, const memory& memory) const {
    switch (arch_) {
    case arch::X86_32:
        return evaluate32(context, memory);
    case arch::X86_64:
        return evaluate64(context, memory);
    }

    ERR("invalid architecture");
    return {};
}

std::unordered_set<uint64_t> case_22::evaluate32(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(value::VALUE_1);
    auto k2_opt   = context.get(value::VALUE_2);
    auto k3_opt   = context.get(value::VALUE_3);
    auto k5_opt   = context.get(value::VALUE_5);
    auto k6_opt   = context.get(value::VALUE_6);
    auto k7_opt   = context.get(value::VALUE_7);
    auto k8_opt   = context.get(value::VALUE_8);

    if (!utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k3_opt, k5_opt, k6_opt, k7_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k5   = k5_opt.value();
    auto k6   = k6_opt.value();
    auto k7   = k7_opt.value();

    if (k3 == k6) {
        ERR("k6 + k10 is not equal to 0");
        return {};
    }

    /* TODO: make sure k8 (32 bit) is a *.get_pc_thunk.* address */

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k5; i++) {
        auto mem_opt = memory.read_i32(k1 + (k7 + rip0) + k2 * i);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (k7 + rip0));
    }

    return values;
}

std::unordered_set<uint64_t> case_22::evaluate64(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(value::VALUE_1);
    auto k2_opt   = context.get(value::VALUE_2);
    auto k3_opt   = context.get(value::VALUE_3);
    auto k5_opt   = context.get(value::VALUE_5);
    auto k6_opt   = context.get(value::VALUE_6);
    auto k7_opt   = context.get(value::VALUE_7);

    if (!utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k3_opt, k5_opt, k6_opt, k7_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k5   = k5_opt.value();
    auto k6   = k6_opt.value();
    auto k7   = k7_opt.value();

    if (k3 == k6) {
        ERR("k6 + k10 is not equal to 0");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k5; i++) {
        auto mem_opt = memory.read_i32(k1 + (k7 + rip0) + k2 * i);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (k7 + rip0));
    }

    return values;
}

} /* namespace libresolver::cases */