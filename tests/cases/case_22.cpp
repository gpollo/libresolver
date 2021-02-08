#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_22 {

TEST_CASE("match-case-22-x86-32") {
    /* create dummy memory */
    dummy::memory memory;
    memory.i32_ = {{1112, 10}, {1116, 20}, {1120, 30}, {1124, 40}, {1128, 50}, {1132, 60}};

    /* dissasemble instructions */
    x86::engine engine(libresolver::arch::X86_32);
    const char bytes[] =
        "call 1000\n"
        "add  $3000,%ebx\n"
        "cmpl $7,70(%edx)\n"
        "ja   10\n"
        "mov  70(%edx),%ecx\n"
        "mov  40(%ebx,%ecx,4),%eax\n"
        "add  %ebx,%eax\n"
        "jmp  *%eax\n";
    auto instructions = engine.assemble_and_disassemble(bytes, 0, 8, 8);

    /* match the instructions */
    libresolver::pattern::tree tree = tests::pattern::create_tree(libresolver::arch::X86_32);
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();
    auto matches                        = matcher.match_instructions(instructions->get());
    auto values                         = matches[0]->evaluate(context, memory);

    REQUIRE(matches.size() == 1);
    REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
    REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBX);
    REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RCX);
    REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RDX);
    REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
    REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
    REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
    REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
    REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
    REQUIRE(context.get(value::VALUE_1).value_or(-1) == 40);
    REQUIRE(context.get(value::VALUE_2).value_or(-1) == 4);
    REQUIRE(context.get(value::VALUE_3).value_or(-1) == 70);
    REQUIRE(context.get(value::VALUE_4).value_or(-1) == 10);
    REQUIRE(context.get(value::VALUE_5).value_or(-1) == 7);
    REQUIRE(context.get(value::VALUE_6).value_or(-1) == 70);
    REQUIRE(context.get(value::VALUE_7).value_or(-1) == 3000);
    REQUIRE(context.get(value::VALUE_8).value_or(-1) == 1000);
    REQUIRE(context.get(value::VALUE_9).value_or(-1) == -1);
    REQUIRE(context.get(value::VALUE_10).value_or(-1) == -1);
    REQUIRE(context.get_rip(0).value_or(-1) == -1);
    REQUIRE(values.size() == 51);
    REQUIRE(values.contains(1010));
    REQUIRE(values.contains(1020));
    REQUIRE(values.contains(1030));
    REQUIRE(values.contains(1040));
    REQUIRE(values.contains(1050));
    REQUIRE(values.contains(1060));
}

} /* namespace tests::cases::case_22 */
