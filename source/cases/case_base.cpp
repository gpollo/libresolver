#include <libresolver/cases/case_base.hpp>

namespace libresolver::cases {

case_base::case_base(const std::string& name, arch arch) : name_(name), arch_(arch) {}

std::string case_base::get_name() const {
    return name_;
}

} /* namespace libresolver::cases */