#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_10.hpp>
#include <libresolver/cases/case_11.hpp>
#include <libresolver/cases/case_12.hpp>
#include <libresolver/cases/case_13.hpp>
#include <libresolver/cases/case_14.hpp>
#include <libresolver/cases/case_15.hpp>
#include <libresolver/cases/case_16.hpp>
#include <libresolver/cases/case_17.hpp>
#include <libresolver/cases/case_18.hpp>
#include <libresolver/cases/case_19.hpp>
#include <libresolver/cases/case_2.hpp>
#include <libresolver/cases/case_20.hpp>
#include <libresolver/cases/case_21.hpp>
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
void add_pattern(libresolver::pattern::tree& tree, libresolver::arch arch) {
    auto new_case = std::make_shared<PatternCase>(arch);
    REQUIRE(new_case != nullptr);
    REQUIRE(tree.add(new_case->get_pattern(), new_case));
}

libresolver::pattern::tree create_tree(libresolver::arch arch) {
    libresolver::pattern::tree tree;

    add_pattern<libresolver::cases::case_1>(tree, arch);
    add_pattern<libresolver::cases::case_2>(tree, arch);
    add_pattern<libresolver::cases::case_3>(tree, arch);
    add_pattern<libresolver::cases::case_4>(tree, arch);
    add_pattern<libresolver::cases::case_5>(tree, arch);
    add_pattern<libresolver::cases::case_6>(tree, arch);
    add_pattern<libresolver::cases::case_7>(tree, arch);
    add_pattern<libresolver::cases::case_8>(tree, arch);
    add_pattern<libresolver::cases::case_9>(tree, arch);
    add_pattern<libresolver::cases::case_10>(tree, arch);
    add_pattern<libresolver::cases::case_11>(tree, arch);
    add_pattern<libresolver::cases::case_12>(tree, arch);
    add_pattern<libresolver::cases::case_13>(tree, arch);
    add_pattern<libresolver::cases::case_14>(tree, arch);
    add_pattern<libresolver::cases::case_15>(tree, arch);
    add_pattern<libresolver::cases::case_16>(tree, arch);
    add_pattern<libresolver::cases::case_17>(tree, arch);
    add_pattern<libresolver::cases::case_18>(tree, arch);
    add_pattern<libresolver::cases::case_19>(tree, arch);
    add_pattern<libresolver::cases::case_20>(tree, arch);
    add_pattern<libresolver::cases::case_21>(tree, arch);

    std::cout << std::to_string(tree) << std::endl;

    return tree;
}

} /* namespace tests::pattern */
