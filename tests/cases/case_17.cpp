#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_17 {

TEST_CASE("pattern tree should match pattern case 17", "[libresolver::cases::case_17]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x8d\x41\x06"                 /* lea    6(%rcx),%eax */
            "\x83\xf9\xfb"                 /* cmp    $-5,%ecx */
            "\x0f\x82\xe5\xee\xff\xff"     /* jb     10 */
            "\x48\x8d\x1d\xe8\x03\x00\x00" /* lea    1000(%rip),%rbx */
            "\x48\x63\x44\x83\x64"         /* movslq 100(%rbx,%rax,4),%rax */
            "\x48\x01\xd8"                 /* add    %rbx,%rax */
            "\xff\xe0";                    /* jmpq   *%rax */

        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{1123, 10}, {1127, 20}, {1131, 30}, {1135, 40}, {1139, 50}, {1143, 60}};

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
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 18446744073709547249);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == static_cast<uint64_t>(-5));
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 6);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 19);
        REQUIRE(values.size() == 6);
        REQUIRE(values.contains(1029));
        REQUIRE(values.contains(1039));
        REQUIRE(values.contains(1049));
        REQUIRE(values.contains(1059));
        REQUIRE(values.contains(1069));
        REQUIRE(values.contains(1079));
    }
}

} /* namespace tests::cases::case_17 */
