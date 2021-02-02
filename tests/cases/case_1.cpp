#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_1 {

TEST_CASE("pattern tree should match pattern case 1", "[libresolver::cases::case_1]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.i32_ = {{{1351, 10}, {1355, 20}, {1359, 30}, {1363, 40}, {1367, 50}, {1371, 60}}};

        const char bytes[] =
            "cmp    $5,%ecx\n"
            "ja     10\n"
            "mov    %ecx,%eax\n"
            "lea    1337(%rip),%rbx\n"
            "movslq (%rbx,%rax,4),%rax\n"
            "add    %rbx,%rax\n"
            "jmpq   *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 7, 7);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBX);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RCX);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1337);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 5);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 14);
        REQUIRE(values.size() == 6);
        REQUIRE(values.contains(1361));
        REQUIRE(values.contains(1371));
        REQUIRE(values.contains(1381));
        REQUIRE(values.contains(1391));
        REQUIRE(values.contains(1401));
        REQUIRE(values.contains(1411));
    }

    SECTION("example 2") {
        memory.i32_ = {
            {{1351, 20}, {1355, 30}, {1359, 40}, {1363, 50}, {1367, 60}, {1371, 70}, {1375, 80}, {1379, 90}}};

        const char bytes[] =
            "cmp    $7,%eax\n"
            "ja     10\n"
            "mov    %eax,%esi\n"
            "lea    1337(%rip),%rdx \n"
            "movslq (%rdx,%rsi,4),%rsi\n"
            "add    %rdx,%rsi\n"
            "jmpq   *%rsi\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 7, 7);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RSI);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RDX);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1337);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 7);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 14);
        REQUIRE(values.size() == 8);
        REQUIRE(values.contains(1371));
        REQUIRE(values.contains(1381));
        REQUIRE(values.contains(1391));
        REQUIRE(values.contains(1401));
        REQUIRE(values.contains(1411));
        REQUIRE(values.contains(1421));
        REQUIRE(values.contains(1431));
        REQUIRE(values.contains(1441));
    }
}

} /* namespace tests::cases::case_1 */
