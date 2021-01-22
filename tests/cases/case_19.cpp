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
        const unsigned char bytes[] =
            "\x83\xf8\x08"                     /* cmp  $8,%eax */
            "\x0f\x87\xe8\xee\xff\xff"         /* ja   10 */
            "\x89\xc0"                         /* mov  %eax,%eax */
            "\x48\x8d\x1c\x85\xe8\x03\x00\x00" /* lea  1000(,%rax,4),%rbx */
            "\x48\x8d\x05\x2c\x01\x00\x00"     /* lea  300(%rip),%rax */
            "\x8b\x44\x03\x64"                 /* mov  100(%rbx,%rax,1),%eax */
            "\x48\x98"                         /* cltq */
            "\x48\x8d\x1d\xe8\x03\x00\x00"     /* lea  1000(%rip),%rbx */
            "\x48\x01\xd8"                     /* add  %rbx,%rax */
            "\xff\xe0";                        /* jmpq *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{1426, 10}, {1430, 20}, {1434, 30}, {1438, 40}, {1442, 50},
                       {1446, 60}, {1450, 70}, {1454, 80}, {1458, 90}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 10);
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
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 39);
        REQUIRE(context.get_rip(1).value_or(-1) == 26);
        REQUIRE(values.size() == 9);
        REQUIRE(values.contains(1049));
        REQUIRE(values.contains(1059));
        REQUIRE(values.contains(1069));
        REQUIRE(values.contains(1079));
        REQUIRE(values.contains(1089));
        REQUIRE(values.contains(1099));
        REQUIRE(values.contains(1109));
        REQUIRE(values.contains(1119));
        REQUIRE(values.contains(1129));
    }
}

} /* namespace tests::cases::case_19 */
