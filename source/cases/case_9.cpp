#include <libresolver/cases/case_9.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_9::case_9(arch arch) : case_base("case-9", arch) {}

pattern::tree::pattern case_9::get_pattern() const {
    /*
     * lea    k7(%rip),%r2
     * cmpl   $k5,k6(%r3)
     * ja     k4
     * mov    k3(%r3),%r1d
     * movslq k1(%r2,%r1,k2),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * where k3 == k6
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        movslq(mem4(K1, R2, R1, K2), reg(R1), {}),
        mov(mem2(K3, R3), reg(R1D), {IGNORE_R1, TRACK_INS_JA}),
        ja(imm(K4), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K5), mem2(K6, R3), {IGNORE_INS_CMP}),
        lea(mem2(K7, RIP), reg(R2), {IGNORE_R2}),
    };
}

std::unordered_set<uint64_t> case_9::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k5_opt   = context.get(K5);
    auto k6_opt   = context.get(K6);
    auto k7_opt   = context.get(K7);

    if (!libresolver::utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k3_opt, k5_opt, k6_opt, k7_opt)) {
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

    if (k3 != k6) {
        ERR("k3 is not equal to k6");
        return {};
    }

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k5; i++) {
        auto mem_opt = memory.read_i32(k1 + (rip0 + k7) + k2 * i);
        if (!mem_opt.has_value()) {
            ERR("failed to read memory");
            return {};
        }

        values.insert(mem_opt.value() + (rip0 + k7));
    }

    return values;
}

} /* namespace libresolver::cases */