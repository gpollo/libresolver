#ifndef LIBRESOLVER_INCLUDE_CONTEXT_HPP
#define LIBRESOLVER_INCLUDE_CONTEXT_HPP

#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>

#include <libresolver/instruction.hpp>
#include <libresolver/placeholder.hpp>
#include <libresolver/utils/registers.hpp>
#include <libresolver/utils/unordered_bimap.hpp>

namespace libresolver {

class context {
   public:
    utils::unordered_bimap<placeholder::reg, x86_reg> allocated_registers_;
    std::unordered_map<placeholder::value, int64_t> allocated_values_;
    std::vector<uint64_t> allocated_rip_values_;

    context() {
        reset();
    }

    void reset() {
        current_register_ = placeholder::reg::REG_1;
        current_value_    = placeholder::value::VALUE_1;

        allocated_registers_.clear();
        allocated_values_.clear();
        allocated_rip_values_.clear();

        pending_registers_.clear();
        pending_values_.clear();
        pending_rip_values_       = std::queue<uint64_t>();
        pending_current_register_ = current_register_;
        pending_current_value_    = current_value_;
        needs_validation_         = false;
    }

    std::optional<x86_reg> get(placeholder::reg reg) const {
        auto reg_opt = allocated_registers_.at(reg);
        if (!reg_opt.has_value()) {
            return {};
        }

        return reg_opt.value();
    }

    std::optional<uint64_t> get(placeholder::value value) const {
        if (!allocated_values_.contains(value)) {
            return {};
        }

        return allocated_values_.at(value);
    }

    std::optional<uint64_t> get_rip(unsigned int index) const {
        if (allocated_rip_values_.size() <= index) {
            return {};
        }

        return allocated_rip_values_[index];
    }

    std::optional<instruction::base> build_instruction(cs_insn& insn) {
        if (!has_last_instruction_been_validated()) {
            std::cerr << "[context::build_instruction] last instruction has not been validated" << std::endl;
            return {};
        }

        x86_insn id = static_cast<x86_insn>(insn.id);
        if (insn.detail == nullptr) {
            std::cerr << "[context::build_instruction] instruction details are missing" << std::endl;
            return {};
        }

        needs_validation_   = true;
        cs_x86_op* operands = insn.detail->x86.operands;

        switch (id) {
        case x86_insn::X86_INS_ADD:
            return create_instruction<instruction::add>(insn, operands[0], operands[1]);
        case x86_insn::X86_INS_CMP:
            return create_instruction<instruction::cmp>(insn, operands[0], operands[1]);
        case x86_insn::X86_INS_CDQE:
            return create_instruction<instruction::cltq>();
        case x86_insn::X86_INS_JA:
            return create_instruction<instruction::ja>(insn, operands[0]);
        case x86_insn::X86_INS_JMP:
            return create_instruction<instruction::jmp>(insn, operands[0]);
        case x86_insn::X86_INS_LEA:
            return create_instruction<instruction::lea>(insn, operands[0], operands[1]);
        case x86_insn::X86_INS_MOV:
            return create_instruction<instruction::mov>(insn, operands[0], operands[1]);
        case x86_insn::X86_INS_MOVZX:
            return create_instruction<instruction::movzx>(insn, operands[0], operands[1]);
        case x86_insn::X86_INS_MOVSXD:
            return create_instruction<instruction::movsxd>(insn, operands[0], operands[1]);
        default:
            std::cerr << "[context::build_instruction] unknown instruction id `" << id << "`" << std::endl;
            return {};
        }
    }

    bool has_last_instruction_been_validated() const {
        return !needs_validation_;
    }

    void validate_last_instruction() {
        for (const auto& pair : pending_registers_.map_t1()) {
            allocated_registers_.insert(pair);
        }
        pending_registers_.clear();

        for (const auto& pair : pending_values_) {
            allocated_values_.insert(pair);
        }
        pending_values_.clear();

        while (!pending_rip_values_.empty()) {
            allocated_rip_values_.push_back(pending_rip_values_.front());
            pending_rip_values_.pop();
        }

        current_register_ = pending_current_register_;
        current_value_    = pending_current_value_;
        needs_validation_ = false;
    }

    void unvalidate_last_instruction() {
        pending_registers_.clear();
        pending_values_.clear();
        pending_rip_values_       = std::queue<uint64_t>();
        pending_current_register_ = current_register_;
        pending_current_value_    = current_value_;
        needs_validation_         = false;
    }

   private:
    placeholder::reg current_register_ = placeholder::reg::REG_1;

    const std::unordered_map<placeholder::reg, placeholder::reg> next_register_ = {
        {placeholder::reg::REG_1, placeholder::reg::REG_2}, {placeholder::reg::REG_2, placeholder::reg::REG_3},
        {placeholder::reg::REG_3, placeholder::reg::REG_4}, {placeholder::reg::REG_4, placeholder::reg::REG_5},
        {placeholder::reg::REG_5, placeholder::reg::REG_6}, {placeholder::reg::REG_6, placeholder::reg::REG_7},
        {placeholder::reg::REG_7, placeholder::reg::REG_8}, {placeholder::reg::REG_8, placeholder::reg::REG_9},
        {placeholder::reg::REG_9, placeholder::reg::NONE},
    };

    placeholder::value current_value_ = placeholder::value::VALUE_1;

    const std::unordered_map<placeholder::value, placeholder::value> next_value_ = {
        {placeholder::value::VALUE_1, placeholder::value::VALUE_2},
        {placeholder::value::VALUE_2, placeholder::value::VALUE_3},
        {placeholder::value::VALUE_3, placeholder::value::VALUE_4},
        {placeholder::value::VALUE_4, placeholder::value::VALUE_5},
        {placeholder::value::VALUE_5, placeholder::value::VALUE_6},
        {placeholder::value::VALUE_6, placeholder::value::VALUE_7},
        {placeholder::value::VALUE_7, placeholder::value::VALUE_8},
        {placeholder::value::VALUE_8, placeholder::value::VALUE_9},
        {placeholder::value::VALUE_9, placeholder::value::VALUE_10},
        {placeholder::value::VALUE_10, placeholder::value::NONE},
    };

    utils::unordered_bimap<placeholder::reg, x86_reg> pending_registers_;
    std::unordered_map<placeholder::value, int64_t> pending_values_;
    std::queue<uint64_t> pending_rip_values_;

    placeholder::reg pending_current_register_ = current_register_;
    placeholder::value pending_current_value_  = current_value_;

    bool needs_validation_ = false;

    std::optional<placeholder::reg> allocate_register(x86_reg base_reg) {
        auto reg_opt = utils::registers::full_of(base_reg);
        if (!reg_opt.has_value()) {
            std::cout << "[context::allocate_register] unknown register `" << base_reg << "`" << std::endl;
            return {};
        }
        auto reg = reg_opt.value();

        if (allocated_registers_.contains(reg)) {
            return allocated_registers_.at(reg);
        }

        if (pending_registers_.contains(reg)) {
            return pending_registers_.at(reg);
        }

        auto allocated_register = pending_current_register_;

        if (!next_register_.contains(pending_current_register_)) {
            std::cout << "[context::allocate_register] no more register left to allocate" << std::endl;
            return {};
        }

        pending_current_register_ = next_register_.at(pending_current_register_);
        pending_registers_.insert({allocated_register, reg});

        return allocated_register;
    }

    std::optional<placeholder::value> allocate_value(uint64_t value) {
        if (value > std::numeric_limits<int64_t>::max()) {
            std::cout << "[context::allocate_value] value doesn't fit in std::int64_t" << std::endl;
            return {};
        }

        if (!next_value_.contains(pending_current_value_)) {
            std::cout << "[context::allocate_register] no more value left to allocate" << std::endl;
            return {};
        }

        auto allocated_value = pending_current_value_;

        pending_current_value_ = next_value_.at(pending_current_value_);
        pending_values_.insert({allocated_value, value});

        return allocated_value;
    }

    std::optional<placeholder::value> allocate_value(int64_t value) {
        if (!next_value_.contains(pending_current_value_)) {
            std::cout << "[context::allocate_register] no more value left to allocate" << std::endl;
            return {};
        }

        auto allocated_value = pending_current_value_;

        pending_current_value_ = next_value_.at(pending_current_value_);
        pending_values_.insert({allocated_value, value});

        return allocated_value;
    }

    template <typename T>
    T create_instruction() {
        return T();
    }

    template <typename T>
    std::optional<T> create_instruction(cs_insn& insn, cs_x86_op& op) {
        auto op_opt = create_operand(op, insn);
        if (!op_opt.has_value()) {
            return {};
        }

        return T(op_opt.value());
    }

    template <typename T>
    std::optional<T> create_instruction(cs_insn& insn, cs_x86_op& op1, cs_x86_op& op2) {
        auto op1_opt = create_operand(op1, insn);
        if (!op1_opt.has_value()) {
            return {};
        }

        auto op2_opt = create_operand(op2, insn);
        if (!op2_opt.has_value()) {
            return {};
        }

        return T(op1_opt.value(), op2_opt.value());
    }

    std::optional<std::pair<placeholder::reg, utils::registers::size>> get_register(x86_reg reg, cs_insn& insn) {
        auto reg_size_opt = utils::registers::size_of(reg);
        if (!reg_size_opt.has_value()) {
            return {};
        }

        if (utils::registers::is_instruction_pointer(reg)) {
            pending_rip_values_.push(insn.address + insn.size);
            return std::make_pair(placeholder::reg::RIP, reg_size_opt.value());
        }

        if (!allocated_registers_.contains(reg)) {
            auto allocated_register_opt = allocate_register(reg);
            if (!allocated_register_opt.has_value()) {
                return {};
            }

            return std::make_pair(allocated_register_opt.value(), reg_size_opt.value());
        }

        return std::make_pair(allocated_registers_.at(reg).value(), reg_size_opt.value());
    }

    std::optional<operand::base_ptr> create_operand_reg(cs_x86_op& op, cs_insn& insn) {
        auto op_reg_opt = get_register(op.reg, insn);
        if (!op_reg_opt.has_value()) {
            return {};
        }
        auto op_reg = op_reg_opt.value();

        return operand::make_reg(op_reg.first, op_reg.second);
    }

    std::optional<operand::base_ptr> create_operand_imm(cs_x86_op& op) {
        auto value_opt = allocate_value(op.imm);
        if (!value_opt.has_value()) {
            return {};
        }
        auto value = value_opt.value();

        return operand::make_imm(value);
    }

    std::optional<operand::base_ptr> create_operand_mem2(x86_op_mem& op, cs_insn& insn) {
        auto disp_value_opt = allocate_value(op.disp);
        if (!disp_value_opt.has_value()) {
            return {};
        }
        auto disp_value = disp_value_opt.value();

        auto base_reg_opt = get_register(op.base, insn);
        if (!base_reg_opt.has_value()) {
            return {};
        }
        auto base_reg = base_reg_opt.value();

        return operand::make_mem2(disp_value, base_reg.first, base_reg.second);
    }

    std::optional<operand::base_ptr> create_operand_mem3(x86_op_mem& op, cs_insn& insn) {
        auto disp_value_opt = allocate_value(op.disp);
        if (!disp_value_opt.has_value()) {
            return {};
        }
        auto disp_value = disp_value_opt.value();

        auto index_reg_opt = get_register(op.index, insn);
        if (!index_reg_opt.has_value()) {
            return {};
        }
        auto index_reg = index_reg_opt.value();

        auto scale_value_opt = allocate_value(static_cast<int64_t>(op.scale));
        if (!scale_value_opt.has_value()) {
            return {};
        }
        auto scale_value = scale_value_opt.value();

        return operand::make_mem3(disp_value, index_reg.first, index_reg.second, scale_value);
    }

    std::optional<operand::base_ptr> create_operand_mem4(x86_op_mem& op, cs_insn& insn) {
        auto disp_value_opt = allocate_value(op.disp);
        if (!disp_value_opt.has_value()) {
            return {};
        }
        auto disp_value = disp_value_opt.value();

        auto base_reg_opt = get_register(op.base, insn);
        if (!base_reg_opt.has_value()) {
            return {};
        }
        auto base_reg = base_reg_opt.value();

        auto index_reg_opt = get_register(op.index, insn);
        if (!index_reg_opt.has_value()) {
            return {};
        }
        auto index_reg = index_reg_opt.value();

        auto scale_value_opt = allocate_value(static_cast<int64_t>(op.scale));
        if (!scale_value_opt.has_value()) {
            return {};
        }
        auto scale_value = scale_value_opt.value();

        return operand::make_mem4(disp_value, base_reg.first, base_reg.second, index_reg.first, index_reg.second,
                                  scale_value);
    }

    std::optional<operand::base_ptr> create_operand_mem(x86_op_mem& op, cs_insn& insn) {
        if (op.segment != x86_reg::X86_REG_INVALID) {
            std::cout << "[context::create_operand_mem] unsupported segment register" << std::endl;
            return {};
        }

        if (op.base != x86_reg::X86_REG_INVALID && op.index == x86_reg::X86_REG_INVALID) {
            return create_operand_mem2(op, insn);
        }

        if (op.base == x86_reg::X86_REG_INVALID && op.index != x86_reg::X86_REG_INVALID) {
            return create_operand_mem3(op, insn);
        }

        if (op.base != x86_reg::X86_REG_INVALID && op.index != x86_reg::X86_REG_INVALID) {
            return create_operand_mem4(op, insn);
        }

        std::cout << "[context::create_operand_mem] unsupported memory operand" << std::endl;
        return {};
    }

    std::optional<operand::base_ptr> create_operand(cs_x86_op& op, cs_insn& insn) {
        switch (op.type) {
        case x86_op_type::X86_OP_INVALID:
            return {};
        case x86_op_type::X86_OP_REG:
            return create_operand_reg(op, insn);
        case x86_op_type::X86_OP_IMM:
            return create_operand_imm(op);
        case x86_op_type::X86_OP_MEM:
            return create_operand_mem(op.mem, insn);
        }

        return {};
    }
};

} /* namespace libresolver */

#endif /* LIBRESOLVER_INCLUDE_CONTEXT_HPP */