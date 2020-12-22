#ifndef LIBRESOLVER_INCLUDE_PATTERN_TREE_HPP
#define LIBRESOLVER_INCLUDE_PATTERN_TREE_HPP

#include <memory>
#include <utility>
#include <vector>

#include <libresolver/instruction.hpp>
#include <libresolver/pattern/actions.hpp>
#include <libresolver/pattern/match.hpp>
#include <libresolver/pattern/node.hpp>

namespace libresolver::pattern {

class tree {
   public:
    using pattern_node = std::pair<instruction::base, actions>;
    using pattern      = std::vector<pattern_node>;

    bool add(const pattern& pattern, const std::shared_ptr<match> match);
    const node& get_root() const;

   private:
    node root_;
};

} /* namespace libresolver::pattern */

namespace std {

string to_string(const libresolver::pattern::tree& tree);

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_PATTERN_TREE_HPP */