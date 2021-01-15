#include <iostream>
#include <string>

#include <libresolver/log.hpp>
#include <libresolver/memory.hpp>

namespace libresolver {

template <typename OutputType>
static inline std::optional<OutputType> read(uint64_t addr) {
    if (addr == 0) {
        return {};
    }

    /* TODO: add memory page checks */

    return *((OutputType*)addr);
}

template <typename OutputType>
static inline std::optional<OutputType> read_debug(uint64_t addr, bool debug, const std::string& type) {
    auto value_opt = read<OutputType>(addr);

    if (debug) {
        std::string result;
        if (value_opt.has_value()) {
            result = std::to_string(+value_opt.value());
        } else {
            result = "failed";
        }

        LOG("(" << type << ") "
                << "0x" << std::hex << addr << " => " << result);
    }

    return value_opt;
}

std::optional<uint8_t> process_memory::read_u8(uint64_t addr) const {
    return read_debug<uint8_t>(addr, debug_, "uint8_t");
}

std::optional<uint16_t> process_memory::read_u16(uint64_t addr) const {
    return read_debug<uint16_t>(addr, debug_, "uint16_t");
}

std::optional<uint32_t> process_memory::read_u32(uint64_t addr) const {
    return read_debug<uint32_t>(addr, debug_, "uint32_t");
}

std::optional<uint64_t> process_memory::read_u64(uint64_t addr) const {
    return read_debug<uint64_t>(addr, debug_, "uint64_t");
}

std::optional<int8_t> process_memory::read_i8(uint64_t addr) const {
    return read_debug<int8_t>(addr, debug_, "int8_t");
}

std::optional<int16_t> process_memory::read_i16(uint64_t addr) const {
    return read_debug<int16_t>(addr, debug_, "int16_t");
}

std::optional<int32_t> process_memory::read_i32(uint64_t addr) const {
    return read_debug<int32_t>(addr, debug_, "int32_t");
}

std::optional<int64_t> process_memory::read_i64(uint64_t addr) const {
    return read_debug<int64_t>(addr, debug_, "int64_t");
}

} /* namespace libresolver */