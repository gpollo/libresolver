#ifndef LIBRESOLVER_INCLUDE_UTILS_HASH_HPP
#define LIBRESOLVER_INCLUDE_UTILS_HASH_HPP

#include <functional>

namespace libresolver::utils::hash {

template <typename T>
size_t combine(size_t seed, const std::reference_wrapper<T> value) {
    return seed ^ (std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template <typename T>
size_t combine(size_t seed, const T& value) {
    return seed ^ (std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template <typename T, typename... Args>
size_t combine(size_t seed, const T& value, const Args&... args) {
    return combine(combine(seed, value), args...);
}

template <typename Iterator>
size_t combine(size_t seed, Iterator begin, Iterator end) {
    for (auto it = begin; it != end; it++) {
        seed = combine(seed, *it);
    }

    return seed;
}

} /* namespace libresolver::utils::hash */

#endif /* LIBRESOLVER_INCLUDE_HASH_HPP */