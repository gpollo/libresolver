#include <libresolver/cases/case_15.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

namespace libresolver::cases {

using libresolver::placeholder::reg;
using libresolver::placeholder::value;
using libresolver::utils::registers::size;

pattern::tree::pattern case_15::get_pattern() const {
    /*
     * cmpl   $k9,k10(%r3)
     * ja     k8
     * mov    k7(%r3),%r1d
     * lea    k5(,%r1,k6),%r2
     * lea    k4(%rip),%r1
     * mov    k2(%r2,%r1,k3),%r1d
     * cltq
     * lea    k1(%rip),%r2
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * where k7 == k10.
     */
    return {
        /* clang-format off */
        {
            instruction::jmp(
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
                .track_regs_ = {reg::REG_1},
            }
        }, {
            instruction::add(
                operand::make_reg(reg::REG_2, size::QWORD),
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
                .track_regs_ = {reg::REG_2},
            }
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_1, reg::RIP, size::QWORD),
                operand::make_reg(reg::REG_2, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_2},
            }
        }, {
            instruction::cltq(), {}
        }, {
            instruction::mov(
                operand::make_mem4(value::VALUE_2, reg::REG_2, size::QWORD, reg::REG_1, size::QWORD, value::VALUE_3),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
                .track_regs_ = {reg::REG_2},
            }
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_4, reg::RIP, size::QWORD),
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_1},
            }
        }, {
            instruction::lea(
                operand::make_mem3(value::VALUE_5, reg::REG_1, size::QWORD, value::VALUE_6),
                operand::make_reg(reg::REG_2, size::QWORD)
            ), {
                .track_regs_ = {reg::REG_1},
                .ignore_regs_ = {reg::REG_2},
            }
        }, {
            instruction::mov(
                operand::make_mem2(value::VALUE_7, reg::REG_3, size::QWORD),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
                .track_regs_ = {reg::REG_3},
                .ignore_regs_ = {reg::REG_1},
                .track_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::ja(
                operand::make_imm(value::VALUE_8)
            ), {
                .track_insns_ = {X86_INS_CMP},
                .ignore_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::cmp(
                operand::make_imm(value::VALUE_9),
                operand::make_mem2(value::VALUE_10, reg::REG_3, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_3},
                .ignore_insns_ = {X86_INS_CMP},
            }
        }
        /* clang-format on */
    };
}

std::string case_15::get_name() const {
    return "case-15";
}

std::unordered_set<uint64_t> case_15::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto rip1_opt = context.get_rip(1);
    auto k1_opt   = context.get(value::VALUE_1);
    auto k2_opt   = context.get(value::VALUE_2);
    auto k3_opt   = context.get(value::VALUE_3);
    auto k4_opt   = context.get(value::VALUE_4);
    auto k5_opt   = context.get(value::VALUE_5);
    auto k6_opt   = context.get(value::VALUE_6);
    auto k7_opt   = context.get(value::VALUE_7);
    auto k9_opt   = context.get(value::VALUE_9);
    auto k10_opt  = context.get(value::VALUE_10);

    if (!utils::optional::all_value(rip0_opt, rip1_opt, k1_opt, k2_opt, k3_opt, k4_opt, k5_opt, k6_opt, k7_opt, k9_opt,
                                    k10_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto rip1 = rip1_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k4   = k4_opt.value();
    auto k5   = k5_opt.value();
    auto k6   = k6_opt.value();
    auto k7   = k7_opt.value();
    auto k9   = k9_opt.value();
    auto k10  = k10_opt.value();

    if (k7 != k10) {
        ERR("k7 is not equal to k10");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k9; i++) {
        auto mem_opt = memory.read_i32(k2 + (k5 + k6 * i) + k3 * (rip1 + k4));
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (rip0 + k1));
    }

    return values;
}

} /* namespace libresolver::cases */