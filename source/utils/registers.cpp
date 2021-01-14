#include <unordered_map>
#include <unordered_set>

#include <libresolver/utils/registers.hpp>

namespace libresolver::utils::registers {

static const std::unordered_map<x86_reg, x86_reg> REGS_32BITS_TO_64BITS = {
    {X86_REG_EAX, X86_REG_RAX},  {X86_REG_EBX, X86_REG_RBX},  {X86_REG_ECX, X86_REG_RCX},  {X86_REG_EDX, X86_REG_RDX},
    {X86_REG_ESP, X86_REG_RSP},  {X86_REG_EBP, X86_REG_RBP},  {X86_REG_EDI, X86_REG_RDI},  {X86_REG_ESI, X86_REG_RSI},
    {X86_REG_R8D, X86_REG_R8},   {X86_REG_R9D, X86_REG_R9},   {X86_REG_R10D, X86_REG_R10}, {X86_REG_R11D, X86_REG_R11},
    {X86_REG_R12D, X86_REG_R12}, {X86_REG_R13D, X86_REG_R13}, {X86_REG_R14D, X86_REG_R14}, {X86_REG_R15D, X86_REG_R15},
    {X86_REG_EIP, X86_REG_RIP},
};

static const std::unordered_map<x86_reg, x86_reg> REGS_16BITS_TO_64BITS = {
    {X86_REG_AX, X86_REG_RAX},   {X86_REG_BX, X86_REG_RBX},   {X86_REG_CX, X86_REG_RCX},   {X86_REG_DX, X86_REG_RDX},
    {X86_REG_SP, X86_REG_RSP},   {X86_REG_BP, X86_REG_RBP},   {X86_REG_DI, X86_REG_RDI},   {X86_REG_SI, X86_REG_RSI},
    {X86_REG_R8W, X86_REG_R8},   {X86_REG_R9W, X86_REG_R9},   {X86_REG_R10W, X86_REG_R10}, {X86_REG_R11W, X86_REG_R11},
    {X86_REG_R12W, X86_REG_R12}, {X86_REG_R13W, X86_REG_R13}, {X86_REG_R14W, X86_REG_R14}, {X86_REG_R15W, X86_REG_R15},
    {X86_REG_IP, X86_REG_RIP},
};

static const std::unordered_map<x86_reg, x86_reg> REGS_8BITS_TO_64BITS = {
    {X86_REG_AL, X86_REG_RAX},   {X86_REG_BL, X86_REG_RBX},   {X86_REG_CL, X86_REG_RCX},   {X86_REG_DL, X86_REG_RDX},
    {X86_REG_SPL, X86_REG_RSP},  {X86_REG_BPL, X86_REG_RBP},  {X86_REG_DIL, X86_REG_RDI},  {X86_REG_SIL, X86_REG_RSI},
    {X86_REG_R8B, X86_REG_R8},   {X86_REG_R9B, X86_REG_R9},   {X86_REG_R10B, X86_REG_R10}, {X86_REG_R11B, X86_REG_R11},
    {X86_REG_R12B, X86_REG_R12}, {X86_REG_R13B, X86_REG_R13}, {X86_REG_R14B, X86_REG_R14}, {X86_REG_R15B, X86_REG_R15},
};

static const std::unordered_map<x86_reg, std::vector<x86_reg>> REGS_GROUP = {
    {X86_REG_RAX, {X86_REG_RAX, X86_REG_EAX, X86_REG_AX, X86_REG_AL}},
    {X86_REG_RBX, {X86_REG_RBX, X86_REG_EBX, X86_REG_BX, X86_REG_BL}},
    {X86_REG_RCX, {X86_REG_RCX, X86_REG_ECX, X86_REG_CX, X86_REG_CL}},
    {X86_REG_RDX, {X86_REG_RDX, X86_REG_EDX, X86_REG_DX, X86_REG_DL}},
    {X86_REG_RSP, {X86_REG_RSP, X86_REG_ESP, X86_REG_SP, X86_REG_SPL}},
    {X86_REG_RBP, {X86_REG_RBP, X86_REG_EBP, X86_REG_BP, X86_REG_BPL}},
    {X86_REG_RDI, {X86_REG_RDI, X86_REG_EDI, X86_REG_DI, X86_REG_DIL}},
    {X86_REG_RSI, {X86_REG_RSI, X86_REG_ESI, X86_REG_SI, X86_REG_SIL}},
    {X86_REG_R8, {X86_REG_R8, X86_REG_R8D, X86_REG_R8W, X86_REG_R8B}},
    {X86_REG_R9, {X86_REG_R9, X86_REG_R9D, X86_REG_R9W, X86_REG_R9B}},
    {X86_REG_R10, {X86_REG_R10, X86_REG_R10D, X86_REG_R10W, X86_REG_R10B}},
    {X86_REG_R11, {X86_REG_R11, X86_REG_R11D, X86_REG_R11W, X86_REG_R11B}},
    {X86_REG_R12, {X86_REG_R12, X86_REG_R12D, X86_REG_R12W, X86_REG_R12B}},
    {X86_REG_R13, {X86_REG_R13, X86_REG_R13D, X86_REG_R13W, X86_REG_R13B}},
    {X86_REG_R14, {X86_REG_R14, X86_REG_R14D, X86_REG_R14W, X86_REG_R14B}},
    {X86_REG_R15, {X86_REG_R15, X86_REG_R15D, X86_REG_R15W, X86_REG_R15B}},
    {X86_REG_RIP, {X86_REG_RIP, X86_REG_EIP, X86_REG_IP}},
};

static const std::unordered_set<x86_reg> INSTRUCTION_POINTERS = {
    {X86_REG_RIP, X86_REG_EIP, X86_REG_IP},
};

bool is_instruction_pointer(x86_reg reg) {
    return INSTRUCTION_POINTERS.contains(reg);
}

std::optional<size> size_of(x86_reg reg) {
    if (REGS_GROUP.contains(reg)) {
        return size::QWORD;
    }

    if (REGS_32BITS_TO_64BITS.contains(reg)) {
        return size::DWORD;
    }

    if (REGS_16BITS_TO_64BITS.contains(reg)) {
        return size::WORD;
    }

    if (REGS_8BITS_TO_64BITS.contains(reg)) {
        return size::BYTE;
    }

    return {};
}

std::optional<std::vector<x86_reg>> written_of(const cs_insn& insn) {
    if (insn.id == x86_insn::X86_INS_CDQE) {
        return {{X86_REG_RAX}};
    }

    if (insn.detail == nullptr) {
        return {};
    }

    std::vector<x86_reg> regs;

    const auto& x86 = insn.detail->x86;
    for (unsigned int i = 0; i < x86.op_count; i++) {
        const auto& op = x86.operands[i];

        if ((op.access & cs_ac_type::CS_AC_WRITE) == 0) {
            continue;
        }

        switch (op.type) {
        case x86_op_type::X86_OP_REG:
            regs.push_back(op.reg);
            break;
        case x86_op_type::X86_OP_IMM:
        case x86_op_type::X86_OP_MEM:
            break;
        case x86_op_type::X86_OP_INVALID:
        default:
            return {};
        }
    }

    return regs;
}

std::optional<std::vector<x86_reg>> group_of(x86_reg base_reg) {
    std::optional<x86_reg> reg_64bits;
    if (REGS_GROUP.contains(base_reg)) {
        reg_64bits = base_reg;
    }

    if (!reg_64bits.has_value() && REGS_32BITS_TO_64BITS.contains(base_reg)) {
        reg_64bits = REGS_32BITS_TO_64BITS.at(base_reg);
    }

    if (!reg_64bits.has_value() && REGS_16BITS_TO_64BITS.contains(base_reg)) {
        reg_64bits = REGS_16BITS_TO_64BITS.at(base_reg);
    }

    if (!reg_64bits.has_value() && REGS_8BITS_TO_64BITS.contains(base_reg)) {
        reg_64bits = REGS_8BITS_TO_64BITS.at(base_reg);
    }

    if (!reg_64bits.has_value()) {
        return {};
    }

    return REGS_GROUP.at(reg_64bits.value());
}

std::optional<x86_reg> full_of(x86_reg reg) {
    if (REGS_GROUP.contains(reg)) {
        return reg;
    }

    if (REGS_32BITS_TO_64BITS.contains(reg)) {
        return REGS_32BITS_TO_64BITS.at(reg);
    }

    if (REGS_16BITS_TO_64BITS.contains(reg)) {
        return REGS_16BITS_TO_64BITS.at(reg);
    }

    if (REGS_8BITS_TO_64BITS.contains(reg)) {
        return REGS_8BITS_TO_64BITS.at(reg);
    }

    return {};
}

} /* namespace libresolver::utils::registers */

namespace std {

string to_string(const libresolver::utils::registers::size& size) {
    switch (size) {
    case libresolver::utils::registers::size::QWORD:
        return "[64]";
    case libresolver::utils::registers::size::DWORD:
        return "[32]";
    case libresolver::utils::registers::size::WORD:
        return "[16]";
    case libresolver::utils::registers::size::BYTE:
        return "[8]";
    }

    return "(INVALID-SIZE)";
}

} /* namespace std */