#include <libresolver/cases/case_14.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

namespace libresolver::cases {

using libresolver::placeholder::reg;
using libresolver::placeholder::value;
using libresolver::utils::registers::size;

pattern::tree::pattern case_14::get_pattern() const {
    /*
     * lea k5(%rip),%r2
     * cmp $k4,%r3b
     * ja k3
     * movslq k1(%r2,%r3,k2),%r1
     * add %r2,%r1
     * jmpq *%r1
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
            instruction::movslq(
                operand::make_mem4(value::VALUE_1, reg::REG_2, size::QWORD, reg::REG_3, size::QWORD, value::VALUE_2),
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
                .track_regs_ = {reg::REG_3},
                .ignore_regs_ = {reg::REG_1},
                .track_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::ja(
                operand::make_imm(value::VALUE_3)
            ), {
                .track_insns_ = {X86_INS_CMP},
                .ignore_insns_ = {X86_INS_JA},
            }
        }, {
            instruction::cmp(
                operand::make_imm(value::VALUE_4),
                operand::make_reg(reg::REG_3, size::BYTE)
            ), {
                .ignore_regs_ = {reg::REG_3},
                .ignore_insns_ = {X86_INS_CMP},
            }
        }, {
            instruction::lea(
                operand::make_mem2(value::VALUE_5, reg::RIP, size::QWORD),
                operand::make_reg(reg::REG_2, size::QWORD)
            ), {
                .ignore_regs_ = {reg::REG_2},
            }
        }
        /* clang-format on */
    };
}

std::string case_14::get_name() const {
    return "case-14";
}

std::unordered_set<uint64_t> case_14::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(value::VALUE_1);
    auto k2_opt   = context.get(value::VALUE_2);
    auto k4_opt   = context.get(value::VALUE_4);
    auto k5_opt   = context.get(value::VALUE_5);

    if (!utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k4_opt, k5_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k4   = k4_opt.value();
    auto k5   = k5_opt.value();

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k4; i++) {
        auto mem_opt = memory.read_i32(k1 + (rip0 + k5) + k2 * i);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (rip0 + k5));
    }

    return values;
}

} /* namespace libresolver::cases */