#include <libresolver/cases/case_24.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_24::case_24(arch arch) : case_base("case-24", arch) {}

pattern::tree::pattern case_24::get_pattern() const {
    /*
     * Similar to case #3, but reuses a register.
     *
     * cmpl   $k6,k7(%r2)
     * ja     k5
     * mov    k4(%r2),%r1d
     * lea    k3(%rip),%r2
     * movslq k1(%r2,%r1,k2),%r1
     * add    %r2,%r1
     * jmp    *%r1
     *
     * where k4 + k7 == 0.
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        movslq(mem4(K1, R2, R1, K2), reg(R1), {}),
        lea(mem2(K3, RIP), reg(R2), {IGNORE_R2}),
        mov(mem2(K4, R2), reg(R1D), {IGNORE_R1, TRACK_R2, TRACK_INS_JA}),
        ja(imm(K5), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K6), mem2(K7, R2), {IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_24::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k4_opt   = context.get(K4);
    auto k6_opt   = context.get(K6);
    auto k7_opt   = context.get(K7);

    if (!libresolver::utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k3_opt, k4_opt, k6_opt, k7_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k4   = k4_opt.value();
    auto k6   = k6_opt.value();
    auto k7   = k7_opt.value();

    if (k4 != k7) {
        ERR("k4 is not equal to k7");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k6; i++) {
        auto mem_opt = memory.read_i32(k1 + (rip0 + k3) + k2 * i);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (rip0 + k3));
    }

    return values;
}

} /* namespace libresolver::cases */