#include <map>
#include <set>
#include <vector>

#include <libresolver/log.hpp>
#include <libresolver/utils/jumps.hpp>

namespace libresolver::utils {

static const std::unordered_set<x86_insn> X86_JUMP_IDS = {
    X86_INS_JAE, X86_INS_JA,  X86_INS_JBE, X86_INS_JB, X86_INS_JCXZ,  X86_INS_JECXZ, X86_INS_JE,
    X86_INS_JGE, X86_INS_JG,  X86_INS_JLE, X86_INS_JL, X86_INS_JMP,   X86_INS_JNE,   X86_INS_JNO,
    X86_INS_JNP, X86_INS_JNS, X86_INS_JO,  X86_INS_JP, X86_INS_JRCXZ, X86_INS_JS,
};

static std::optional<std::reference_wrapper<const cs_x86_op>> get_jump_operand(const cs_insn& insn) {
    if (insn.detail == nullptr) {
        ERR("instruction details not available");
        return {};
    }

    auto& x86 = insn.detail->x86;

    if (x86.op_count != 1) {
        ERR("invalid number of operands `" << x86.op_count << "`");
        return {};
    }

    return x86.operands[0];
}

std::optional<jumps> jumps::create(const std::vector<std::reference_wrapper<cs_insn>>& insns) {
    std::map<uint64_t, unsigned int> address_to_index;
    std::vector<std::pair<unsigned int, uint64_t>> direct_jump_to_address;
    std::unordered_map<unsigned int, unsigned int> direct_jumps;
    std::unordered_set<unsigned int> indirect_jumps;

    for (unsigned int i = 0; i < insns.size(); i++) {
        auto& insn = insns[i].get();

        address_to_index.insert({insn.address, i});

        if (!X86_JUMP_IDS.contains(static_cast<x86_insn>(insn.id))) {
            continue;
        }

        auto op_opt = get_jump_operand(insn);
        if (!op_opt.has_value()) {
            return {};
        }
        auto& op = op_opt.value().get();

        switch (op.type) {
        case x86_op_type::X86_OP_REG:
        case x86_op_type::X86_OP_MEM:
            indirect_jumps.insert(i);
            break;
        case x86_op_type::X86_OP_IMM:
            direct_jump_to_address.push_back({i, op.imm});
            break;
        case x86_op_type::X86_OP_INVALID:
        default:
            ERR("operand is invalid");
            return {};
        }
    }

    // for (auto [addr, index] : address_to_index) {
    //     std::cout << "contains: " << addr << "|" << index << std::endl;
    // }

    for (auto [source_index, target_address] : direct_jump_to_address) {
        // std::cout << "get: " << target_address << std::endl;
        if (!address_to_index.contains(target_address)) {
            ERR("instruction doesn't land on an instruction");
            return {};
        }

        direct_jumps.insert({source_index, address_to_index.at(target_address)});
    }

    return jumps(direct_jumps, indirect_jumps);
}

jumps::jumps(const std::unordered_map<unsigned int, unsigned int>& direct_jumps,
             const std::unordered_set<unsigned int>& indirect_jumps)
    : direct_jumps_(direct_jumps), indirect_jumps_(indirect_jumps) {}

} /* namespace libresolver::utils */