#ifndef LIBRESOLVER_INCLUDE_CASES_CASE_BASE_HPP
#define LIBRESOLVER_INCLUDE_CASES_CASE_BASE_HPP

#include <libresolver/arch.hpp>
#include <libresolver/pattern/match.hpp>
#include <libresolver/pattern/tree.hpp>

namespace libresolver::cases {

class case_base : public pattern::match {
   public:
    case_base(const std::string& name, arch arch);
    std::string get_name() const override;

   protected:
    const std::string name_;
    const arch arch_;
};

} /* namespace libresolver::cases */

#endif /* LIBRESOLVER_INCLUDE_CASES_CASE_BASE_HPP */