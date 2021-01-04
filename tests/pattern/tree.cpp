#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_10.hpp>
#include <libresolver/cases/case_11.hpp>
#include <libresolver/cases/case_12.hpp>
#include <libresolver/cases/case_13.hpp>
#include <libresolver/cases/case_14.hpp>
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

    auto case_8 = std::make_shared<libresolver::cases::case_8>();
    REQUIRE(case_8 != nullptr);
    REQUIRE(tree.add(case_8->get_pattern(), case_8));

    auto case_9 = std::make_shared<libresolver::cases::case_9>();
    REQUIRE(case_9 != nullptr);
    REQUIRE(tree.add(case_9->get_pattern(), case_9));

    auto case_10 = std::make_shared<libresolver::cases::case_10>();
    REQUIRE(case_10 != nullptr);
    REQUIRE(tree.add(case_10->get_pattern(), case_10));

    auto case_11 = std::make_shared<libresolver::cases::case_11>();
    REQUIRE(case_11 != nullptr);
    REQUIRE(tree.add(case_11->get_pattern(), case_11));

    auto case_12 = std::make_shared<libresolver::cases::case_12>();
    REQUIRE(case_12 != nullptr);
    REQUIRE(tree.add(case_12->get_pattern(), case_12));

    auto case_13 = std::make_shared<libresolver::cases::case_13>();
    REQUIRE(case_13 != nullptr);
    REQUIRE(tree.add(case_13->get_pattern(), case_13));

    auto case_14 = std::make_shared<libresolver::cases::case_14>();
    REQUIRE(case_14 != nullptr);
    REQUIRE(tree.add(case_14->get_pattern(), case_14));

    return tree;
}

} /* namespace tests::pattern */
