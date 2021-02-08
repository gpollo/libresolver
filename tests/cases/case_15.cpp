#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_15 {

TEST_CASE("match-case-15") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    SECTION("example 1") {
        memory.i32_ = {{144, 10}, {148, 20}, {152, 30}, {156, 40}, {160, 50}, {164, 60}, {168, 70}};

        const char bytes[] =
            "cmpl $6,40(%rbp)\n"
            "ja   30\n"
            "mov  40(%rbp),%eax\n"
            "lea  0(,%rax,4),%rdx\n"
            "lea  20(%rip),%rax\n"
            "mov  100(%rdx,%rax,1),%eax\n"
            "cltq\n"
            "lea  10(%rip),%rdx\n"
            "add  %rdx,%rax\n"
            "jmpq *%rax\n";
        auto instructions = engine.assemble_and_disassemble(bytes, 0, 10, 10);
        auto matches      = matcher.match_instructions(instructions->get());
        auto values       = matches[0]->evaluate(context, memory);

        REQUIRE(matches.size() == 1);
        REQUIRE(context.get(reg::REG_1).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RAX);
        REQUIRE(context.get(reg::REG_2).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RDX);
        REQUIRE(context.get(reg::REG_3).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_RBP);
        REQUIRE(context.get(reg::REG_4).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_5).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_6).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_7).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_8).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(reg::REG_9).value_or(x86_reg::X86_REG_INVALID) == x86_reg::X86_REG_INVALID);
        REQUIRE(context.get(value::VALUE_1).value_or(-1) == 10);
        REQUIRE(context.get(value::VALUE_2).value_or(-1) == 100);
        REQUIRE(context.get(value::VALUE_3).value_or(-1) == 1);
        REQUIRE(context.get(value::VALUE_4).value_or(-1) == 20);
        REQUIRE(context.get(value::VALUE_5).value_or(-1) == 0);
        REQUIRE(context.get(value::VALUE_6).value_or(-1) == 4);
        REQUIRE(context.get(value::VALUE_7).value_or(-1) == 40);
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 30);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 6);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == 40);
        REQUIRE(context.get_rip(0).value_or(-1) == 37);
        REQUIRE(context.get_rip(1).value_or(-1) == 24);
        REQUIRE(values.size() == 7);
        REQUIRE(values.contains(57));
        REQUIRE(values.contains(67));
        REQUIRE(values.contains(77));
        REQUIRE(values.contains(87));
        REQUIRE(values.contains(97));
        REQUIRE(values.contains(107));
        REQUIRE(values.contains(117));
    }

    /* TODO: add a case where k7 != k10 */
}

} /* namespace tests::cases::case_15 */
