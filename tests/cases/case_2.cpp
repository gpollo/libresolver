#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_2 {

TEST_CASE("pattern tree should match pattern case 2", "[libresolver::cases::case_2]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x83\xf9\x05"                     /* cmp    $5,%ecx */
            "\x0f\x87\xe8\xee\xff\xff"         /* ja     10 */
            "\x48\x8d\x1d\xe8\x03\x00\x00"     /* lea    1000(%rip),%rbx */
            "\x89\xc9"                         /* mov    %ecx,%ecx */
            "\x48\x63\x84\x8b\xa4\x01\x00\x00" /* movslq 420(%rbx,%rcx,4),%rax */
            "\x48\x01\xd8"                     /* add    %rbx,%rax */
            "\xff\xe0";                        /* jmpq   *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{{1436, 10}, {1440, 20}, {1444, 30}, {1448, 40}, {1452, 50}}};

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
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 420);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 5);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 16);
        REQUIRE(values.size() == 5);
        REQUIRE(values.contains(1026));
        REQUIRE(values.contains(1036));
        REQUIRE(values.contains(1046));
        REQUIRE(values.contains(1056));
        REQUIRE(values.contains(1066));
    }
}

} /* namespace tests::cases::case_2 */
