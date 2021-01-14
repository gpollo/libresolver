#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_4 {

TEST_CASE("pattern tree should match pattern case 4", "[libresolver::cases::case_4]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x48\x83\xf9\x08"             /* cmp    $8,%rcx */
            "\x0f\x87\xe7\xee\xff\xff"     /* ja     10 */
            "\x48\x8d\x1d\xe8\x03\x00\x00" /* lea    1000(%rip),%rbx */
            "\x48\x63\x44\x8b\x64"         /* movslq 100(%rbx,%rcx,4),%rax */
            "\x48\x01\xd8"                 /* add    %rbx,%rax */
            "\xff\xe0";                    /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{1117, 10}, {1121, 20}, {1125, 30}, {1129, 40}, {1133, 50}, {1137, 60}, {1141, 70}, {1145, 80}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 6);
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
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 17);
        REQUIRE(values.size() == 8);
        REQUIRE(values.contains(1027));
        REQUIRE(values.contains(1037));
        REQUIRE(values.contains(1047));
        REQUIRE(values.contains(1057));
        REQUIRE(values.contains(1067));
        REQUIRE(values.contains(1077));
        REQUIRE(values.contains(1087));
        REQUIRE(values.contains(1097));
    }
}

} /* namespace tests::cases::case_4 */
