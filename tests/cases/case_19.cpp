#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_19 {

TEST_CASE("pattern tree should match pattern case 19", "[libresolver::cases::case_19]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.i32_ = {{1422, 10}, {1426, 20}, {1430, 30}, {1434, 40}, {1438, 50},
                       {1442, 60}, {1446, 70}, {1450, 80}, {1454, 90}};

        const char bytes[] =
            "cmp  $8,%eax\n"
            "ja   10\n"
            "mov  %eax,%eax\n"
            "lea  1000(,%rax,4),%rbx\n"
            "lea  300(%rip),%rax\n"
            "mov  100(%rbx,%rax,1),%eax\n"
            "cltq\n"
            "lea  1000(%rip),%rbx\n"
            "add  %rbx,%rax\n"
            "jmpq *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 10, 10);
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
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 300);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 35);
        REQUIRE(context.get_rip(1).value_or(-1) == 22);
        REQUIRE(values.size() == 9);
        REQUIRE(values.contains(1045));
        REQUIRE(values.contains(1055));
        REQUIRE(values.contains(1065));
        REQUIRE(values.contains(1075));
        REQUIRE(values.contains(1085));
        REQUIRE(values.contains(1095));
        REQUIRE(values.contains(1105));
        REQUIRE(values.contains(1115));
        REQUIRE(values.contains(1125));
    }
}

} /* namespace tests::cases::case_19 */
