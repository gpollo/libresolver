#ifndef LIBRESOLVER_INCLUDE_CASES_CASE_19_HPP
#define LIBRESOLVER_INCLUDE_CASES_CASE_19_HPP

#include <libresolver/pattern/match.hpp>
#include <libresolver/pattern/tree.hpp>

namespace libresolver::cases {

class case_19 : public pattern::match {
   public:
    pattern::tree::pattern get_pattern() const;
    std::string get_name() const override;
    std::unordered_set<uint64_t> evaluate(const context& context, const memory& memory) const override;
};

} /* namespace libresolver::cases */

#endif /* LIBRESOLVER_INCLUDE_CASES_CASE_19_HPP */