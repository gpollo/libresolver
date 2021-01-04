#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_2.hpp>
#include <libresolver/cases/case_3.hpp>
#include <libresolver/cases/case_4.hpp>
#include <libresolver/cases/case_5.hpp>
#include <libresolver/cases/case_6.hpp>
#include <libresolver/cases/case_7.hpp>
#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>

namespace tests::pattern {

libresolver::pattern::tree create_tree() {
    libresolver::pattern::tree tree;

    auto case_1 = std::make_shared<libresolver::cases::case_1>();
    REQUIRE(case_1 != nullptr);
    REQUIRE(tree.add(case_1->get_pattern(), case_1));

    auto case_2 = std::make_shared<libresolver::cases::case_2>();
    REQUIRE(case_2 != nullptr);
    REQUIRE(tree.add(case_2->get_pattern(), case_2));

    auto case_3 = std::make_shared<libresolver::cases::case_3>();
    REQUIRE(case_3 != nullptr);
    REQUIRE(tree.add(case_3->get_pattern(), case_3));

    auto case_4 = std::make_shared<libresolver::cases::case_4>();
    REQUIRE(case_4 != nullptr);
    REQUIRE(tree.add(case_4->get_pattern(), case_4));

    auto case_5 = std::make_shared<libresolver::cases::case_5>();
    REQUIRE(case_5 != nullptr);
    REQUIRE(tree.add(case_5->get_pattern(), case_5));

    auto case_6 = std::make_shared<libresolver::cases::case_6>();
    REQUIRE(case_6 != nullptr);
    REQUIRE(tree.add(case_6->get_pattern(), case_6));

    auto case_7 = std::make_shared<libresolver::cases::case_7>();
    REQUIRE(case_7 != nullptr);
    REQUIRE(tree.add(case_7->get_pattern(), case_7));

    return tree;
}

} /* namespace tests::pattern */
