#ifndef LIBRESOLVER_INCLUDE_PATTERN_MATCH_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_MATCH_HPP

#include <unordered_set>

#include <libresolver/context.hpp>
#include <libresolver/instruction.hpp>
#include <libresolver/memory.hpp>
#include <libresolver/pattern/actions.hpp>

namespace libresolver::pattern {

class match {
   public:
    virtual ~match()                                                                                  = default;
    virtual std::string get_name() const                                                              = 0;
    virtual std::unordered_set<uint64_t> evaluate(const context& context, const memory& memory) const = 0;
};

} /* namespace libresolver::pattern */

#endif /* LIBRESOLVER_INCLUDE_PATTERN_MATCH_HPP */