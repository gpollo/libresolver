#ifndef LIBRESOLVER_INCLUDE_CASES_CASE_22_HPP
#define LIBRESOLVER_INCLUDE_CASES_CASE_22_HPP

#include <libresolver/cases/case_base.hpp>

namespace libresolver::cases {

class case_22 : public case_base {
   public:
    case_22(arch arch);
    pattern::tree::pattern get_pattern() const;
    std::unordered_set<uint64_t> evaluate(const context& context, const memory& memory) const override;
   private:
    std::unordered_set<uint64_t> evaluate32(const context& context, const memory& memory) const;
    std::unordered_set<uint64_t> evaluate64(const context& context, const memory& memory) const;
};

} /* namespace libresolver::cases */

#endif /* LIBRESOLVER_INCLUDE_CASES_CASE_22_HPP */