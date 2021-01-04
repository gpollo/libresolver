#include <libresolver/cases/case_12.hpp>
#include <libresolver/utils/optional.hpp>

namespace libresolver::cases {

using libresolver::placeholder::reg;
using libresolver::placeholder::value;
using libresolver::utils::registers::size;

pattern::tree::pattern case_12::get_pattern() const {
    /*
     * lea    k5(%rip),%r2
     * cmp    $k4,%r1d
     * ja     k3
     * movslq k1(%r2,%r1,k2),%r1
     * add    %r2,%r1
     * jmpq   *%r1
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
            instruction::movsxd(
                operand::make_mem4(value::VALUE_1, reg::REG_2, size::QWORD, reg::REG_1, size::QWORD, value::VALUE_2),
                operand::make_reg(reg::REG_1, size::QWORD)
            ), {
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
                operand::make_reg(reg::REG_1, size::DWORD)
            ), {
                .ignore_regs_ = {reg::REG_1},
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

std::unordered_set<uint64_t> case_12::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(value::VALUE_1);
    auto k2_opt   = context.get(value::VALUE_2);
    auto k4_opt   = context.get(value::VALUE_4);
    auto k5_opt   = context.get(value::VALUE_5);

    if (!utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k4_opt, k5_opt)) {
        std::cerr << "[cases::case_12::evaluate] missing matched values" << std::endl;
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k4   = k4_opt.value();
    auto k5   = k5_opt.value();

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i < k4; i++) {
        auto mem_opt = memory.read_i32(k1 + (rip0 + k5) + k2 * i);
        if (!mem_opt.has_value()) {
            std::cerr << "[cases::case_12::evaluate] failed to read memory" << std::endl;
            return {};
        }

        values.insert(mem_opt.value() + (rip0 + k5));
    }

    return values;
}

} /* namespace libresolver::cases */