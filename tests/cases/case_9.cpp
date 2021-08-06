#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_9 {

TEST_CASE("match-case-9") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.i32_ = {{1107, 10}, {1111, 20}, {1115, 30}, {1119, 40}, {1123, 50},
                       {1127, 60}, {1131, 70}, {1135, 80}, {1139, 90}};

        const char bytes[] =
            "lea    0x3E8(%rip),%rbx\n"
            "cmpl   $8,0x14(%rcx)\n"
            "ja     0xA\n"
            "mov    0x14(%rcx),%eax\n"
            "movslq 0x64(%rbx,%rax,4),%rax\n"
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
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 20);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 8);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 20);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 1000);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
        REQUIRE(context.get_rip(0).value_or(-1) == 7);
        REQUIRE(values.size() == 9);
        REQUIRE(values.contains(1017));
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

} /* namespace tests::cases::case_9 */
