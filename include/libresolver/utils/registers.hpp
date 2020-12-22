#ifndef LIBRESOLVER_INCLUDE_UTILS_REGISTERS_HPP
#define LIBRESOLVER_INCLUDE_UTILS_REGISTERS_HPP

#include <optional>
#include <string>
#include <vector>

#include <capstone/capstone.h>

namespace libresolver::utils::registers {

enum class size {
    QWORD, /* 8 bytes */
    DWORD, /* 4 bytes */
    WORD,  /* 2 bytes */
    BYTE,  /* 1 byte  */
};

bool is_instruction_pointer(x86_reg reg);
std::optional<size> size_of(x86_reg reg);
std::optional<std::vector<x86_reg>> written_of(const cs_insn& insn);
std::optional<std::vector<x86_reg>> group_of(x86_reg base_reg);
std::optional<x86_reg> full_of(x86_reg reg);

} /* namespace libresolver::utils::registers */

namespace std {

string to_string(const libresolver::utils::registers::size& size);

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_UTILS_REGISTERS_HPP */