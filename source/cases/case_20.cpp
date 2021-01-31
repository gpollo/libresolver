#include <libresolver/cases/case_20.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

namespace libresolver::cases {

using libresolver::placeholder::reg;
using libresolver::placeholder::value;
using libresolver::utils::registers::size;

pattern::tree::pattern case_20::get_pattern() const {
    /*
     * For non position-independent code. Jump target addresses
     * are directly stored in the jump table.
     *
     * lea    k10(%r4),%r1d
     * cmp    $k9,%r1b
     * ja     k8
     * movsbl %r4b,%r1d
     * lea    k7(%rip),%r3
     * lea    k6(%rip),%r2
     * sub    $k5,%r1d
     * cltq
     * movzbl k3(%r3,%r1,k4),%r1d
     * mov    k1(%r2,%r1,k2),%r1
     * jmpq   *%r1
     *
     * where k5 + k10 == 0.
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
            instruction::mov(
                operand::make_mem4(value::VALUE_1, reg::REG_2, size::QWORD, reg::REG_1, size::QWORD, value::VALUE_2),
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
                .track_regs_ = {reg::REG_2},
            }
        }, {
            instruction::movzx(
                operand::make_mem4(value::VALUE_3, reg::REG_3, size::QWORD, reg::REG_1, size::QWORD, value::VALUE_4),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
                .track_regs_ = {reg::REG_3},
            }
        }, {
            instruction::cltq(), {}
        }, {
            instruction::sub(
                operand::make_imm(value::VALUE_5),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {}
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_6, reg::RIP, size::QWORD),
                operand::make_reg(reg::REG_2, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_2},
            }
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_7, reg::RIP, size::QWORD),
                operand::make_reg(reg::REG_3, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_3},
            }
        }, {
            instruction::movsbl(
                operand::make_reg(reg::REG_4, size::BYTE),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
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
                operand::make_reg(reg::REG_1, size::BYTE)
            ), {
                .ignore_insns_ = {X86_INS_CMP},
            }
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_10, reg::REG_4, size::QWORD),
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
                .ignore_regs_ = {reg::REG_1, reg::REG_4},
            }
        }
        /* clang-format on */
    };
}

std::string case_20::get_name() const {
    return "case-20";
}

std::unordered_set<uint64_t> case_20::evaluate(const context& context, const memory& memory) const {
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
    auto k10  = static_cast<int64_t>(k10_opt.value());

    if (k5 + k10 != 0) {
        ERR("k5 + k10 is not equal to 0");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k9 - k10; i++) {
        auto mem8_opt = memory.read_u8(k3 + (rip1 + k7) + (i - k5) * k4);
        if (!mem8_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        auto mem_opt = memory.read_u64(k1 + (rip0 + k6) + mem8_opt.value() * k2);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value());
    }

    return values;
}

} /* namespace libresolver::cases */