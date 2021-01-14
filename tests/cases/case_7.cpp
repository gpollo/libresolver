#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_7 {

TEST_CASE("pattern tree should match pattern case 7", "[libresolver::cases::case_7]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x48\x8d\x1d\xe8\x03\x00\x00" /* lea    1000(%rip),%rbx */
            "\x3c\x06"                     /* cmp    $6,%al */
            "\x0f\x87\xe0\xee\xff\xff"     /* ja     10 */
            "\x0f\xb6\xc0"                 /* movzbl %al,%eax */
            "\x48\x63\x44\x83\x64"         /* movslq 100(%rbx,%rax,4),%rax */
            "\x48\x01\xd8"                 /* add    %rbx,%rax */
            "\xff\xe0";                    /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{1107, 10}, {1111, 20}, {1115, 30}, {1119, 40}, {1123, 50}, {1127, 60}};

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
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 18446744073709547247);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 6);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 7);
        REQUIRE(values.size() == 6);
        REQUIRE(values.contains(1017));
        REQUIRE(values.contains(1027));
        REQUIRE(values.contains(1037));
        REQUIRE(values.contains(1047));
        REQUIRE(values.contains(1057));
        REQUIRE(values.contains(1067));
    }
}

} /* namespace tests::cases::case_7 */
