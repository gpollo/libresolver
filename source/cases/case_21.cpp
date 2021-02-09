#include <libresolver/cases/case_21.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_21::case_21(arch arch) : case_base("case-21", arch) {}

pattern::tree::pattern case_21::get_pattern() const {
    /*
     * For non position-independent code. Jump target addresses
     * are directly stored in the jump table.
     *
     * lea    k10(%r3),%r1d
     * cmp    $k9,%r1b
     * ja     k8
     * movsbl %r3b,%r1d
     * lea    k7(%rip),%r2
     * sub    $k6,%r1d
     * cltq
     * movzbl k4(%r2,%r1,k5),%r1d
     * lea    k3(%rip),%r2
     * mov    k1(%r2,%r1,k2),%r1
     * jmpq   *%r1
     *
     * where k6 + k10 == 0.
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        mov(mem4(K1, R2, R1, K2), reg(R1), {TRACK_R2}),
        lea(mem2(K3, RIP), reg(R2), {IGNORE_R2}),
        movzbl(mem4(K4, R2, R1, K5), reg(R1D), {TRACK_R2}),
        cltq({}),
        sub(imm(K6), reg(R1D), {}),
        lea(mem2(K7, RIP), reg(R2), {TRACK_R2}),
        movsbl(reg(R3B), reg(R1D), {TRACK_R3, TRACK_INS_JA}),
        ja(imm(K8), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K9), reg(R1B), {IGNORE_INS_CMP}),
        lea(mem2(K10, R3), reg(R1D), {IGNORE_R1, IGNORE_R3}),
    };
}

std::unordered_set<uint64_t> case_21::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto rip1_opt = context.get_rip(1);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k4_opt   = context.get(K4);
    auto k5_opt   = context.get(K5);
    auto k6_opt   = context.get(K6);
    auto k7_opt   = context.get(K7);
    auto k9_opt   = context.get(K9);
    auto k10_opt  = context.get(K10);

    if (!libresolver::utils::optional::all_value(rip0_opt, rip1_opt, k1_opt, k2_opt, k3_opt, k4_opt, k5_opt, k6_opt,
                                                 k7_opt, k9_opt, k10_opt)) {
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

    if (k6 + k10 != 0) {
        ERR("k6 + k10 is not equal to 0");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k9 - k10; i++) {
        auto mem8_opt = memory.read_u8(k4 + (rip1 + k7) + (i - k6) * k5);
        if (!mem8_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        auto mem_opt = memory.read_u64(k1 + (rip0 + k3) + mem8_opt.value() * k2);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value());
    }

    return values;
}

} /* namespace libresolver::cases */