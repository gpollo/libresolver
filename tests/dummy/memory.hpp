#ifndef LIBRESOLVER_TESTS_DUMMY_MEMORY_HPP
#define LIBRESOLVER_TESTS_DUMMY_MEMORY_HPP

#include <iostream>
#include <unordered_map>

#include <libresolver/memory.hpp>
#include <libresolver/log.hpp>

namespace tests::dummy {

class memory : public libresolver::memory {
public:
    /* TODO: check that expected memory addresses are accessed */
    
    bool debug_ = true;

    std::unordered_map<uint64_t, uint8_t> u8_;
    std::unordered_map<uint64_t, uint16_t> u16_;
    std::unordered_map<uint64_t, uint32_t> u32_;
    std::unordered_map<uint64_t, uint64_t> u64_;
    std::unordered_map<uint64_t, int8_t> i8_;
    std::unordered_map<uint64_t, int16_t> i16_;
    std::unordered_map<uint64_t, int32_t> i32_;
    std::unordered_map<uint64_t, int64_t> i64_;
    
    template <typename OutputType>
    std::optional<OutputType> read(uint64_t addr, const std::unordered_map<uint64_t, OutputType>& mem) const {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            return {};
        }

        return it->second;
    }

    template <typename OutputType>
    std::optional<OutputType> read_debug(uint64_t addr, const std::unordered_map<uint64_t, OutputType>& mem, const std::string& type) const {
        auto value_opt = read(addr, mem);

        if (debug_) {
            std::string result;
            if (value_opt.has_value()) {
                result = std::to_string(+value_opt.value());
            } else {
                result = "failed";
            }
            
            LOG("(" << type << ") " << "0x" << std::hex << addr << " => " << result);
        }

        return value_opt;
    }

    std::optional<uint8_t> read_u8(uint64_t addr) const   override {
        return read_debug(addr, u8_, "uint8_t");
    }

    std::optional<uint16_t> read_u16(uint64_t addr) const override {
        return read_debug(addr, u16_, "uint16_t");
    }

    std::optional<uint32_t> read_u32(uint64_t addr) const override {
        return read_debug(addr, u32_, "uint32_t");
    }

    std::optional<uint64_t> read_u64(uint64_t addr) const override {
        return read_debug(addr, u64_, "uint64_t");
    }

    std::optional<int8_t> read_i8(uint64_t addr) const   override {
        return read_debug(addr, i8_, "int8_t");
    }

    std::optional<int16_t> read_i16(uint64_t addr) const override {
        return read_debug(addr, i16_, "int16_t");
    }

    std::optional<int32_t> read_i32(uint64_t addr) const override {
        return read_debug(addr, i32_, "int32_t");
    }

    std::optional<int64_t> read_i64(uint64_t addr) const override {
        return read_debug(addr, i64_, "int64_t");
    }
};

} /* namespace tests::dummy */

#endif /* LIBRESOLVER_TESTS_DUMMY_MEMORY_HPP */