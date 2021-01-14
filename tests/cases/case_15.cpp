#include <libresolver/pattern/matcher.hpp>
#include <tests/dummy/memory.hpp>
#include <tests/external/catch.hpp>
#include <tests/pattern/tree.hpp>
#include <tests/x86/engine.hpp>

using libresolver::placeholder::reg;
using libresolver::placeholder::value;

namespace tests::cases::case_15 {

TEST_CASE("pattern tree should match pattern case 15", "[libresolver::cases::case_15]") {
    x86::engine engine;
    dummy::memory memory;

    libresolver::pattern::tree tree = tests::pattern::create_tree();
    libresolver::pattern::matcher matcher(tree);
    const libresolver::context& context = matcher.get_context();

    std::cout << std::to_string(tree) << std::endl;

    SECTION("example 1") {
        const unsigned char bytes[] =
            "\x83\x7d\x28\x06"                 /* cmpl $6,40(%rbp) */
            "\x0f\x87\xfb\xee\xff\xff"         /* ja   30 */
            "\x8b\x45\x28"                     /* mov  40(%rbp),%eax */
            "\x48\x8d\x14\x85\x00\x00\x00\x00" /* lea  0(,%rax,4),%rdx */
            "\x48\x8d\x05\x14\x00\x00\x00"     /* lea  20(%rip),%rax */
            "\x8b\x44\x02\x64"                 /* mov  100(%rdx,%rax,1),%eax */
            "\x48\x98"                         /* cltq */
            "\x48\x8d\x15\x0a\x00\x00\x00"     /* lea  10(%rip),%rdx */
            "\x48\x01\xd0"                     /* add  %rdx,%rax */
            "\xff\xe0";                        /* jmpq *%rax */
        const size_t bytes_len = sizeof(bytes) - 1;

        memory.i32_ = {{148, 10}, {152, 20}, {156, 30}, {160, 40}, {164, 50}, {168, 60}, {172, 70}};

        auto instructions = engine.disassemble(bytes, bytes_len, 0, 10);
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
        REQUIRE(context.get(value::VALUE_8).value_or(-1) == 18446744073709547269);
        REQUIRE(context.get(value::VALUE_9).value_or(-1) == 6);
        REQUIRE(context.get(value::VALUE_10).value_or(-1) == 40);
        REQUIRE(context.get_rip(0).value_or(-1) == 41);
        REQUIRE(context.get_rip(1).value_or(-1) == 28);
        REQUIRE(values.size() == 7);
        REQUIRE(values.contains(61));
        REQUIRE(values.contains(71));
        REQUIRE(values.contains(81));
        REQUIRE(values.contains(91));
        REQUIRE(values.contains(101));
        REQUIRE(values.contains(111));
        REQUIRE(values.contains(121));
    }

    /* TODO: add a case where k7 != k10 */
}

} /* namespace tests::cases::case_15 */
