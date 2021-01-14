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
        const unsigned char bytes[] =
            "\x83\xf9\x05"                 /* #0 cmp    $5,%ecx */
            "\x0f\x87\xe8\xee\xff\xff"     /* #1 ja     10 */
            "\x89\xc8"                     /* #2 mov    %ecx,%eax */
            "\x48\x8d\x1d\x39\x05\x00\x00" /* #3 lea    1337(%rip),%rbx */
            "\x48\x63\x04\x83"             /* #4 movslq 0(%rbx,%rax,4),%rax */
            "\x48\x01\xd8"                 /* #5 add    %rbx,%rax */
            "\xff\xe0";                    /* #6 jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{{1355, 10}, {1359, 20}, {1363, 30}, {1367, 40}, {1371, 50}}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 7);
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
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 5);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 18);
        REQUIRE(values.size() == 5);
        REQUIRE(values.contains(1365));
        REQUIRE(values.contains(1375));
        REQUIRE(values.contains(1385));
        REQUIRE(values.contains(1395));
        REQUIRE(values.contains(1405));
    }

    SECTION("example 2") {
        const unsigned char bytes[] =
            "\x83\xf8\x07"                 /* cmp    $7,%eax */
            "\x0f\x87\xe8\xee\xff\xff"     /* ja     10 */
            "\x89\xc6"                     /* mov    %eax,%esi */
            "\x48\x8d\x15\x39\x05\x00\x00" /* lea    1337(%rip),%rdx  */
            "\x48\x63\x34\xb2"             /* movslq 0(%rdx,%rsi,4),%rsi */
            "\x48\x01\xd6"                 /* add    %rdx,%rsi */
            "\xff\xe6";                    /* jmpq   *%rsi */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{{1355, 20}, {1359, 30}, {1363, 40}, {1367, 50}, {1371, 60}, {1375, 70}, {1379, 80}}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 7);
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
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 7);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 18);
        REQUIRE(values.size() == 7);
        REQUIRE(values.contains(1375));
        REQUIRE(values.contains(1385));
        REQUIRE(values.contains(1395));
        REQUIRE(values.contains(1405));
        REQUIRE(values.contains(1415));
        REQUIRE(values.contains(1425));
        REQUIRE(values.contains(1435));
    }
}

} /* namespace tests::cases::case_1 */
