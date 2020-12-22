#ifndef LIBRESOLVER_INCLUDE_UTILS_MEMORY_HPP
#define LIBRESOLVER_INCLUDE_UTILS_MEMORY_HPP

#include <memory>
#include <vector>

namespace libresolver::utils::memory {

template <typename T>
static bool compare_shared_ptr_vector(std::vector<std::shared_ptr<T>> v1, std::vector<std::shared_ptr<T>> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }

    for (unsigned int i = 0; i < v1.size(); i++) {
        if (*v1[i] != *v2[i]) {
            return false;
        }
    }

    return true;
}

} /* namespace libresolver::utils::memory */

#endif /* LIBRESOLVER_INCLUDE_UTILS_MEMORY_HPP */