#include <libresolver/cases/case_22.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_22::case_22(arch arch) : case_base("case-22", arch) {}

pattern::tree::pattern case_22::get_pattern() const {
    /*
     * For non position-independent code. Jump target addresses
     * are directly stored in the jump table.
     *
     * cmp    $k4,%r1d
     * ja     k3
     * mov    %r1d,%r1d
     * mov    k1(,%r1,k2),%r1
     * jmp    *%r1
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        mov(mem3(K1, R1, K2), reg(R1), {}),
        mov(reg(R1D), reg(R1D), {TRACK_INS_JA}),
        ja(imm(K3), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K4), reg(R1D), {IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_22::evaluate(const context& context, const memory& memory) const {
    auto k1_opt = context.get(K1);
    auto k2_opt = context.get(K2);
    auto k4_opt = context.get(K4);

    if (!libresolver::utils::optional::all_value(k1_opt, k2_opt, k4_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto k1 = k1_opt.value();
    auto k2 = k2_opt.value();
    auto k4 = k4_opt.value();

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k4; i++) {
        auto mem_opt = memory.read_u64(k1 + i * k2);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value());
    }

    return values;
}

} /* namespace libresolver::cases */