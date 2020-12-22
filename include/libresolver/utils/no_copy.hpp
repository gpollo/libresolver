#ifndef LIBRESOLVER_INCLUDE_UTILS_NO_COPY_HPP
#define LIBRESOLVER_INCLUDE_UTILS_NO_COPY_HPP

namespace libresolver::utils {

class no_copy {
   protected:
    no_copy()                     = default;
    no_copy(const no_copy& other) = delete;
    no_copy(no_copy&& other)      = delete;
    no_copy& operator=(no_copy&&) = delete;
    no_copy& operator=(const no_copy& other) = delete;
};

} /* namespace libresolver::utils */

#endif /* LIBRESOLVER_INCLUDE_UTILS_NO_COPY_HPP */