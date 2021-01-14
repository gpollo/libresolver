#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_10.hpp>
#include <libresolver/cases/case_11.hpp>
#include <libresolver/cases/case_12.hpp>
#include <libresolver/cases/case_13.hpp>
#include <libresolver/cases/case_14.hpp>
#include <libresolver/cases/case_15.hpp>
#include <libresolver/cases/case_2.hpp>
#include <libresolver/cases/case_3.hpp>
#include <libresolver/cases/case_4.hpp>
#include <libresolver/cases/case_5.hpp>
#include <libresolver/cases/case_6.hpp>
#include <libresolver/cases/case_7.hpp>
#include <libresolver/cases/case_8.hpp>
#include <libresolver/cases/case_9.hpp>
#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>

namespace tests::pattern {

template <typename PatternCase>
void add_pattern(libresolver::pattern::tree& tree) {
    auto new_case = std::make_shared<PatternCase>();
    REQUIRE(new_case != nullptr);
    REQUIRE(tree.add(new_case->get_pattern(), new_case));
}

libresolver::pattern::tree create_tree() {
    libresolver::pattern::tree tree;

    add_pattern<libresolver::cases::case_1>(tree);
    add_pattern<libresolver::cases::case_2>(tree);
    add_pattern<libresolver::cases::case_3>(tree);
    add_pattern<libresolver::cases::case_4>(tree);
    add_pattern<libresolver::cases::case_5>(tree);
    add_pattern<libresolver::cases::case_6>(tree);
    add_pattern<libresolver::cases::case_7>(tree);
    add_pattern<libresolver::cases::case_8>(tree);
    add_pattern<libresolver::cases::case_9>(tree);
    add_pattern<libresolver::cases::case_10>(tree);
    add_pattern<libresolver::cases::case_11>(tree);
    add_pattern<libresolver::cases::case_12>(tree);
    add_pattern<libresolver::cases::case_13>(tree);
    add_pattern<libresolver::cases::case_14>(tree);
    add_pattern<libresolver::cases::case_15>(tree);

    return tree;
}

} /* namespace tests::pattern */
