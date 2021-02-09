#include <libresolver/cases/case_5.hpp>
#include <libresolver/cases/utils.hpp>
#include <libresolver/log.hpp>
#include <libresolver/utils/optional.hpp>

using namespace libresolver::cases::utils;

namespace libresolver::cases {

case_5::case_5(arch arch) : case_base("case-5", arch) {}

pattern::tree::pattern case_5::get_pattern() const {
    /*
     * cmp    $k5,%r4d
     * ja     k4
     * lea    k3(%rip),%r2
     * mov    %r4d,%r3d
     * movslq k1(%r2,%r3,k2),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        jmp(reg(R1), {TRACK_R1}),
        add(reg(R2), reg(R1), {TRACK_R2}),
        movslq(mem4(K1, R2, R3, K2), reg(R1), {TRACK_R3, IGNORE_R1}),
        mov(reg(R4D), reg(R3D), {TRACK_R4, IGNORE_R3}),
        lea(mem2(K3, RIP), reg(R2), {IGNORE_R2, TRACK_INS_JA}),
        ja(imm(K4), {TRACK_INS_CMP, IGNORE_INS_JA}),
        cmp(imm(K5), reg(R4D), {IGNORE_R4, IGNORE_INS_CMP}),
    };
}

std::unordered_set<uint64_t> case_5::evaluate(const context& context, const memory& memory) const {
    auto rip0_opt = context.get_rip(0);
    auto k1_opt   = context.get(K1);
    auto k2_opt   = context.get(K2);
    auto k3_opt   = context.get(K3);
    auto k5_opt   = context.get(K5);

    if (!libresolver::utils::optional::all_value(rip0_opt, k1_opt, k2_opt, k3_opt, k5_opt)) {
        ERR("missing matched values");
        return {};
    }

    auto rip0 = rip0_opt.value();
    auto k1   = k1_opt.value();
    auto k2   = k2_opt.value();
    auto k3   = k3_opt.value();
    auto k5   = k5_opt.value();

    std::unordered_set<uint64_t> values;
    for (unsigned int i = 0; i <= k5; i++) {
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