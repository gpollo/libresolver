#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_11 {

TEST_CASE("pattern tree should match pattern case 11", "[libresolver::cases::case_11]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x3c\x09"                     /* cmp    $9,%al */
            "\x0f\x87\xe9\xee\xff\xff"     /* ja     10 */
            "\x48\x8d\x1d\xe8\x03\x00\x00" /* lea    1000(%rip),%rbx */
            "\x0f\xb6\xc0"                 /* movzbl %al,%eax */
            "\x48\x63\x44\x83\x64"         /* movslq 100(%rbx,%rax,4),%rax */
            "\x48\x01\xd8"                 /* add    %rbx,%rax */
            "\xff\xe0";                    /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{1115, 10}, {1119, 20}, {1123, 30}, {1127, 40}, {1131, 50},
                       {1135, 60}, {1139, 70}, {1143, 80}, {1147, 90}, {1151, 100}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 7);
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
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 9);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 15);
        REQUIRE(values.size() == 10);
        REQUIRE(values.contains(1025));
        REQUIRE(values.contains(1035));
        REQUIRE(values.contains(1045));
        REQUIRE(values.contains(1055));
        REQUIRE(values.contains(1065));
        REQUIRE(values.contains(1075));
        REQUIRE(values.contains(1085));
        REQUIRE(values.contains(1095));
        REQUIRE(values.contains(1105));
        REQUIRE(values.contains(1115));
    }
}

} /* namespace tests::cases::case_11 */
