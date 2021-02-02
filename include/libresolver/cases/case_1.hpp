#ifndef LIBRESOLVER_INCLUDE_CASES_CASE_1_HPP
#define LIBRESOLVER_INCLUDE_CASES_CASE_1_HPP

#include <libresolver/cases/case_base.hpp>

namespace libresolver::cases {

class case_1 : public case_base {
   public:
    case_1(arch arch);
    pattern::tree::pattern get_pattern() const;
    std::unordered_set<uint64_t> evaluate(const context& context, const memory& memory) const override;
};

} /* namespace libresolver::cases */

#endif /* LIBRESOLVER_INCLUDE_CASES_CASE_1_HPP */