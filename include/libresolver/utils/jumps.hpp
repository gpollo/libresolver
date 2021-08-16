#ifndef LIBRESOLVER_INCLUDE_UTILS_JUMPS_HPP
#define LIBRESOLVER_INCLUDE_UTILS_JUMPS_HPP

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include <capstone/capstone.h>

namespace libresolver::utils {

class jumps {
   public:
    static std::optional<jumps> create(const std::vector<std::reference_wrapper<cs_insn>>& insns);

   private:
    const std::unordered_map<unsigned int, unsigned int> direct_jumps_;
    const std::unordered_set<unsigned int> indirect_jumps_;

    jumps(const std::unordered_map<unsigned int, unsigned int>& direct_jumps,
          const std::unordered_set<unsigned int>& indirect_jumps);
};

} /* namespace libresolver::utils */

#endif /* LIBRESOLVER_INCLUDE_UTILS_JUMPS_HPP */