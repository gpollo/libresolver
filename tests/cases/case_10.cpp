#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_10 {

TEST_CASE("match-case-10") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.i32_ = {{1112, 10}, {1116, 20}, {1120, 30}, {1124, 40}, {1128, 50}, {1132, 60}, {1136, 70}};

        const char bytes[] =
            "cmp    $6,%eax\n"
            "ja     10\n"
            "lea    1000(%rip),%rbx\n"
            "movslq 100(%rbx,%rax,4),%rax\n"
            "add    %rbx,%rax\n"
            "jmpq   *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 6, 6);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBX);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 6);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 12);
        REQUIRE(values.size() == 7);
        REQUIRE(values.contains(1022));
        REQUIRE(values.contains(1032));
        REQUIRE(values.contains(1042));
        REQUIRE(values.contains(1052));
        REQUIRE(values.contains(1062));
        REQUIRE(values.contains(1072));
        REQUIRE(values.contains(1082));
    }
}

} /* namespace tests::cases::case_10 */
