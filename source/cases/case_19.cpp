#include <libresolver/cases/case_19.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_19::case_19(arch arch) : case_base("case-19", arch) {}

pattern::tree::pattern case_19::get_pattern() const {
    /*
     * cmp    $k8,%r1d
     * ja     k7
     * mov    %r1d,%r1d
     * lea    k5(,%r1,k6),%r2
     * lea    k4(%rip),%r1
     * mov    k2(%r2,%r1,k3),%r1d
     * cltq
     * lea    k1(%rip),%r2
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        lea(mem2(K1, RIP), reg(R2), {IGNORE_R2}),
        cltq({}),
        mov(mem4(K2, R2, R1, K3), reg(R1D), {TRACK_R2}),
        lea(mem2(K4, RIP), reg(R1), {IGNORE_R1}),
        lea(mem3(K5, R1, K6), reg(R2), {TRACK_R1, IGNORE_R2}),
        mov(reg(R1D), reg(R1D), {TRACK_INS_JA}),
        ja(imm(K7), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K8), reg(R1D), {IGNORE_R1, IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_19::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto rip1_opt = context.get_rip(1);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k4_opt   = context.get(K4);
    auto k5_opt   = context.get(K5);
    auto k6_opt   = context.get(K6);
    auto k8_opt   = context.get(K8);

    if (!libresolver::utils::optional::all_value(rip0_opt, rip1_opt, k1_opt, k2_opt, k3_opt, k4_opt, k5_opt, k6_opt,
                                                 k8_opt)) {
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
    auto k8   = k8_opt.value();

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k8; i++) {
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