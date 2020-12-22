#include <unordered_set>

#include <capstone/capstone.h>
#include <libresolver/context.hpp>
#include <libresolver/pattern/node.hpp>
#include <tests/external/catch.hpp>
#include <tests/x86/data.hpp>

namespace tests::x86 {

TEST_CASE("context should create instructions", "[libresolver::context]") {
    libresolver::context context;

    auto insns = data::get_instructions();
    REQUIRE(insns.size() == 13);

    using libresolver::placeholder::reg;
    using libresolver::placeholder::value;
    using libresolver::utils::registers::size;

    SECTION("should generate libresolver::operand::reg correctly") {
        auto insn_jmp_opt = context.build_instruction(insns[7]);
        REQUIRE(insn_jmp_opt.has_value());
        context.validate_last_instruction();

        auto insn_jmp = insn_jmp_opt.value();
        REQUIRE(insn_jmp.get_insn() == x86_insn::X86_INS_JMP);
        REQUIRE(insn_jmp.get_operands().size() == 1);
        REQUIRE(*insn_jmp.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::QWORD));
    }

    SECTION("should generate libresolver::operand::imm correctly") {
        auto insn_ja_opt = context.build_instruction(insns[1]);
        REQUIRE(insn_ja_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja = insn_ja_opt.value();
        REQUIRE(insn_ja.get_insn() == x86_insn::X86_INS_JA);
        REQUIRE(insn_ja.get_operands().size() == 1);
        REQUIRE(*insn_ja.get_operands()[0] == libresolver::operand::imm(value::VALUE_1));

        REQUIRE(context.allocated_values_.size() == 1);
        REQUIRE(context.allocated_values_.contains(value::VALUE_1));
        REQUIRE(context.allocated_values_.at(value::VALUE_1) == 2123);
    }

    SECTION("should generate libresolver::operand::mem2 correctly") {
        auto insn_lea_opt = context.build_instruction(insns[12]);
        REQUIRE(insn_lea_opt.has_value());
        context.validate_last_instruction();

        auto insn_lea = insn_lea_opt.value();
        REQUIRE(insn_lea.get_insn() == x86_insn::X86_INS_LEA);
        REQUIRE(insn_lea.get_operands().size() == 2);
        REQUIRE(*insn_lea.get_operands()[0] == libresolver::operand::mem2(value::VALUE_1, reg::REG_1, size::QWORD));

        REQUIRE(context.allocated_values_.size() == 1);
        REQUIRE(context.allocated_values_.contains(value::VALUE_1));
        REQUIRE(context.allocated_values_.at(value::VALUE_1) == 0x120c);
    }

    SECTION("should generate libresolver::operand::mem4 correctly") {
        auto insn_movsxd_opt = context.build_instruction(insns[5]);
        REQUIRE(insn_movsxd_opt.has_value());
        context.validate_last_instruction();

        auto insn_movsxd = insn_movsxd_opt.value();
        REQUIRE(insn_movsxd.get_insn() == x86_insn::X86_INS_MOVSXD);
        REQUIRE(insn_movsxd.get_operands().size() == 2);
        REQUIRE(*insn_movsxd.get_operands()[0] == libresolver::operand::mem4(value::VALUE_1, reg::REG_1, size::QWORD,
                                                                             reg::REG_2, size::QWORD, value::VALUE_2));

        REQUIRE(context.allocated_values_.size() == 2);
        REQUIRE(context.allocated_values_.contains(value::VALUE_1));
        REQUIRE(context.allocated_values_.at(value::VALUE_1) == 0);
        REQUIRE(context.allocated_values_.contains(value::VALUE_2));
        REQUIRE(context.allocated_values_.at(value::VALUE_2) == 4);
    }

    SECTION("should generate same instructions if they use the same registers") {
        auto insn_ja_1_opt = context.build_instruction(insns[7]);
        REQUIRE(insn_ja_1_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_2_opt = context.build_instruction(insns[7]);
        REQUIRE(insn_ja_2_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_1 = insn_ja_1_opt.value();
        auto insn_ja_2 = insn_ja_2_opt.value();
        REQUIRE(insn_ja_1.get_insn() == x86_insn::X86_INS_JMP);
        REQUIRE(insn_ja_2.get_insn() == x86_insn::X86_INS_JMP);
        REQUIRE(insn_ja_1.get_operands().size() == 1);
        REQUIRE(insn_ja_2.get_operands().size() == 1);
        REQUIRE(*insn_ja_1.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::QWORD));
        REQUIRE(*insn_ja_2.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::QWORD));
    }

    SECTION("should generate different instructions if they use different registers") {
        auto insn_ja_1_opt = context.build_instruction(insns[7]);
        REQUIRE(insn_ja_1_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_2_opt = context.build_instruction(insns[8]);
        REQUIRE(insn_ja_2_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_1 = insn_ja_1_opt.value();
        auto insn_ja_2 = insn_ja_2_opt.value();
        REQUIRE(insn_ja_1.get_insn() == x86_insn::X86_INS_JMP);
        REQUIRE(insn_ja_2.get_insn() == x86_insn::X86_INS_JMP);
        REQUIRE(insn_ja_1.get_operands().size() == 1);
        REQUIRE(insn_ja_2.get_operands().size() == 1);
        REQUIRE(*insn_ja_1.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::QWORD));
        REQUIRE(*insn_ja_2.get_operands()[0] == libresolver::operand::reg(reg::REG_2, size::QWORD));
    }

    SECTION("should generate the same register with different sizes") {
        auto insn_mov_1_opt = context.build_instruction(insns[9]);
        REQUIRE(insn_mov_1_opt.has_value());
        context.validate_last_instruction();

        auto insn_mov_2_opt = context.build_instruction(insns[10]);
        REQUIRE(insn_mov_2_opt.has_value());
        context.validate_last_instruction();

        auto insn_mov_1 = insn_mov_1_opt.value();
        auto insn_mov_2 = insn_mov_2_opt.value();
        REQUIRE(insn_mov_1.get_insn() == x86_insn::X86_INS_MOV);
        REQUIRE(insn_mov_2.get_insn() == x86_insn::X86_INS_MOV);
        REQUIRE(insn_mov_1.get_operands().size() == 2);
        REQUIRE(insn_mov_2.get_operands().size() == 2);
        REQUIRE(*insn_mov_1.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::QWORD));
        REQUIRE(*insn_mov_2.get_operands()[0] == libresolver::operand::reg(reg::REG_1, size::DWORD));
        REQUIRE(*insn_mov_1.get_operands()[1] == libresolver::operand::reg(reg::REG_2, size::QWORD));
        REQUIRE(*insn_mov_2.get_operands()[1] == libresolver::operand::reg(reg::REG_2, size::DWORD));
    }

    SECTION("should allocate different values") {
        auto insn_ja_1_opt = context.build_instruction(insns[1]);
        REQUIRE(insn_ja_1_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_2_opt = context.build_instruction(insns[1]);
        REQUIRE(insn_ja_2_opt.has_value());
        context.validate_last_instruction();

        auto insn_ja_1 = insn_ja_1_opt.value();
        auto insn_ja_2 = insn_ja_2_opt.value();
        REQUIRE(insn_ja_1.get_insn() == x86_insn::X86_INS_JA);
        REQUIRE(insn_ja_2.get_insn() == x86_insn::X86_INS_JA);
        REQUIRE(insn_ja_1.get_operands().size() == 1);
        REQUIRE(insn_ja_2.get_operands().size() == 1);
        REQUIRE(*insn_ja_1.get_operands()[0] == libresolver::operand::imm(value::VALUE_1));
        REQUIRE(*insn_ja_2.get_operands()[0] == libresolver::operand::imm(value::VALUE_2));
    }

    SECTION("should allocate different instruction pointer values") {
        auto insn_lea_1_opt = context.build_instruction(insns[2]);
        REQUIRE(insn_lea_1_opt.has_value());
        context.validate_last_instruction();

        auto insn_lea_2_opt = context.build_instruction(insns[11]);
        REQUIRE(insn_lea_2_opt.has_value());
        context.validate_last_instruction();

        auto insn_lea_1 = insn_lea_1_opt.value();
        auto insn_lea_2 = insn_lea_2_opt.value();
        REQUIRE(insn_lea_1.get_insn() == x86_insn::X86_INS_LEA);
        REQUIRE(insn_lea_2.get_insn() == x86_insn::X86_INS_LEA);
        REQUIRE(insn_lea_1.get_operands().size() == 2);
        REQUIRE(insn_lea_2.get_operands().size() == 2);
        REQUIRE(*insn_lea_1.get_operands()[0] == libresolver::operand::mem2(value::VALUE_1, reg::RIP, size::QWORD));
        REQUIRE(*insn_lea_2.get_operands()[0] == libresolver::operand::mem2(value::VALUE_2, reg::RIP, size::QWORD));
        REQUIRE(*insn_lea_1.get_operands()[1] == libresolver::operand::reg(reg::REG_1, size::QWORD));
        REQUIRE(*insn_lea_2.get_operands()[1] == libresolver::operand::reg(reg::REG_1, size::QWORD));
        REQUIRE(context.allocated_rip_values_.size() == 2);
        REQUIRE(context.allocated_rip_values_[0] == 2012);
        REQUIRE(context.allocated_rip_values_[1] == 2040);
    }
}

} /* namespace tests::x86 */