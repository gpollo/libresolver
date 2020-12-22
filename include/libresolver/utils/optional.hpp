#ifndef LIBRESOLVER_INCLUDE_UTILS_OPTIONAL_HPP
#define LIBRESOLVER_INCLUDE_UTILS_OPTIONAL_HPP

namespace libresolver::utils::optional {

static inline bool all_value() {
    return true;
}

template <typename T, typename... Args>
static bool all_value(const T& opt, const Args&... args) {
    if (!opt.has_value()) {
        return false;
    }

    return all_value(args...);
}

} /* namespace libresolver::utils::optional */

#endif /* LIBRESOLVER_INCLUDE_UTILS_OPTIONAL_HPP */