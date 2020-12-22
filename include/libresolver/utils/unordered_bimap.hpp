#ifndef LIBRESOLVER_INCLUDE_UTILS_UNORDERED_BIMAP_HPP
#define LIBRESOLVER_INCLUDE_UTILS_UNORDERED_BIMAP_HPP

namespace libresolver::utils {

/**
 * Very basic implementation of a two-way map.
 * Don't use it unless you know what you're doing.
 */
template <typename T1, typename T2>
class unordered_bimap {
   public:
    void clear() noexcept {
        map_t1_.clear();
        map_t2_.clear();
    }

    bool contains(const T1& k) const {
        return map_t1_.contains(k);
    }

    bool contains(const T2& k) const {
        return map_t2_.contains(k);
    }

    void insert(const T1& v1, const T2& v2) {
        map_t1_.insert({v1, v2});
        map_t2_.insert({v2, v1});
    }

    void insert(const T2& v2, const T1& v1) {
        insert(v1, v2);
    }

    void insert(const std::pair<T2, T1>& p) {
        insert(p.first, p.second);
    }

    void insert(const std::pair<T1, T2>& p) {
        insert(p.first, p.second);
    }

    std::optional<std::reference_wrapper<T2>> at(const T1& k) {
        if (!contains(k)) {
            return {};
        }

        return map_t1_.at(k);
    }

    std::optional<std::reference_wrapper<T1>> at(const T2& k) {
        if (!contains(k)) {
            return {};
        }

        return map_t2_.at(k);
    }

    std::optional<std::reference_wrapper<const T2>> at(const T1& k) const {
        if (!contains(k)) {
            return {};
        }

        return map_t1_.at(k);
    }

    std::optional<std::reference_wrapper<const T1>> at(const T2& k) const {
        if (!contains(k)) {
            return {};
        }

        return map_t2_.at(k);
    }

    const std::unordered_map<T1, T2> map_t1() const {
        return map_t1_;
    }

    const std::unordered_map<T2, T1> map_t2() const {
        return map_t2_;
    }

   private:
    std::unordered_map<T1, T2> map_t1_;
    std::unordered_map<T2, T1> map_t2_;
};

} /* namespace libresolver::utils */

#endif /* LIBRESOLVER_INCLUDE_UTILS_UNORDERED_BIMAP_HPP */