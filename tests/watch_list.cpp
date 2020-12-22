#include <capstone/capstone.h>
#include <libresolver/watch_list.hpp>
#include <tests/external/catch.hpp>
#include <tests/x86/data.hpp>

namespace tests::x86 {

TEST_CASE("watch list should track registers and instructions", "[libresolver::watch_list]") {
    libresolver::watch_list watch_list;

    auto insns = data::get_instructions();
    REQUIRE(insns.size() == 13);

    SECTION("should not track anything") {
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(!watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(!watch_list.matches(insns[3]));
        REQUIRE(!watch_list.matches(insns[4]));
        REQUIRE(!watch_list.matches(insns[5]));
        REQUIRE(!watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));
    }

    SECTION("should track registers") {
        REQUIRE(watch_list.add_register(x86_reg::X86_REG_DX));
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(!watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(!watch_list.matches(insns[3]));
        REQUIRE(!watch_list.matches(insns[4]));
        REQUIRE(watch_list.matches(insns[5]));
        REQUIRE(watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));

        REQUIRE(watch_list.remove_register(x86_reg::X86_REG_RDX));
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(!watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(!watch_list.matches(insns[3]));
        REQUIRE(!watch_list.matches(insns[4]));
        REQUIRE(!watch_list.matches(insns[5]));
        REQUIRE(!watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));
    }

    SECTION("should track instructions") {
        watch_list.add_instruction(x86_insn::X86_INS_JA);
        watch_list.add_instruction(x86_insn::X86_INS_MOV);
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(watch_list.matches(insns[3]));
        REQUIRE(watch_list.matches(insns[4]));
        REQUIRE(!watch_list.matches(insns[5]));
        REQUIRE(!watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));

        watch_list.remove_instruction(x86_insn::X86_INS_JA);
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(!watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(watch_list.matches(insns[3]));
        REQUIRE(watch_list.matches(insns[4]));
        REQUIRE(!watch_list.matches(insns[5]));
        REQUIRE(!watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));

        watch_list.remove_instruction(x86_insn::X86_INS_MOV);
        REQUIRE(!watch_list.matches(insns[0]));
        REQUIRE(!watch_list.matches(insns[1]));
        REQUIRE(!watch_list.matches(insns[2]));
        REQUIRE(!watch_list.matches(insns[3]));
        REQUIRE(!watch_list.matches(insns[4]));
        REQUIRE(!watch_list.matches(insns[5]));
        REQUIRE(!watch_list.matches(insns[6]));
        REQUIRE(!watch_list.matches(insns[7]));
    }
}

} /* namespace tests::x86 */