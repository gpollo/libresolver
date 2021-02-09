#include <libresolver/cases/case_8.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_8::case_8(arch arch) : case_base("case-8", arch) {}

pattern::tree::pattern case_8::get_pattern() const {
    /*
     * cmpl   $k6,k7(%rip)
     * ja     k5
     * mov    k4(%rip),%r1d
     * lea    k3(%rip),%r2
     * movslq k1(%r2,%r1,k2),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * where rip1 + k4 == rip2 + k7
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        movslq(mem4(K1, R2, R1, K2), reg(R1), {}),
        lea(mem2(K3, RIP), reg(R2), {IGNORE_R2}),
        mov(mem2(K4, RIP), reg(R1D), {IGNORE_R1, TRACK_INS_JA}),
        ja(imm(K5), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K6), mem2(K7, RIP), {IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_8::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto rip1_opt = context.get_rip(1);
    auto rip2_opt = context.get_rip(2);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k4_opt   = context.get(K4);
    auto k6_opt   = context.get(K6);
    auto k7_opt   = context.get(K7);

    if (!libresolver::utils::optional::all_value(rip0_opt, rip1_opt, rip2_opt, k1_opt, k2_opt, k3_opt, k4_opt, k6_opt,
                                                 k7_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto rip1 = rip1_opt.value();
    auto rip2 = rip2_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k4   = k4_opt.value();
    auto k6   = k6_opt.value();
    auto k7   = k7_opt.value();

    if (rip1 + k4 != rip2 + k7) {
        ERR("rip1 + k4 is not equal to rip2 + k7");
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