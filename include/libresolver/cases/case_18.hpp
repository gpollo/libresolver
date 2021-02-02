#ifndef LIBRESOLVER_INCLUDE_CASES_CASE_18_HPP
#define LIBRESOLVER_INCLUDE_CASES_CASE_18_HPP

#include <libresolver/cases/case_base.hpp>

namespace libresolver::cases {

class case_18 : public case_base {
   public:
    case_18(arch arch);
    pattern::tree::pattern get_pattern() const;
    std::unordered_set<uint64_t> evaluate(const context& context, const memory& memory) const override;
};

} /* namespace libresolver::cases */

#endif /* LIBRESOLVER_INCLUDE_CASES_CASE_18_HPP */