#include <libresolver/cases/case_15.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_15::case_15(arch arch) : case_base("case-15", arch) {}

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
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        lea(mem2(K1, RIP), reg(R2), {TRACK_R2}),
        cltq({}),
        mov(mem4(K2, R2, R1, K3), reg(R1D), {TRACK_R2}),
        lea(mem2(K4, RIP), reg(R1), {IGNORE_R1}),
        lea(mem3(K5, R1, K6), reg(R2), {TRACK_R1, IGNORE_R2}),
        mov(mem2(K7, R3), reg(R1D), {TRACK_R3, IGNORE_R1, TRACK_INS_JA}),
        ja(imm(K8), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K9), mem2(K10, R3), {IGNORE_R3, IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_15::evaluate(const context& context, const memory& memory) const {
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