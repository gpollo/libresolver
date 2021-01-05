#ifndef LIBRESOLVER_INCLUDE_MEMORY_HPP
#define LIBRESOLVER_INCLUDE_MEMORY_HPP

#include <cstdint>
#include <optional>

namespace libresolver {

class memory {
   public:
    virtual ~memory() = default;

    virtual std::optional<uint8_t> read_u8(uint64_t addr) const   = 0;
    virtual std::optional<uint16_t> read_u16(uint64_t addr) const = 0;
    virtual std::optional<uint32_t> read_u32(uint64_t addr) const = 0;
    virtual std::optional<uint64_t> read_u64(uint64_t addr) const = 0;

    virtual std::optional<int8_t> read_i8(uint64_t addr) const   = 0;
    virtual std::optional<int16_t> read_i16(uint64_t addr) const = 0;
    virtual std::optional<int32_t> read_i32(uint64_t addr) const = 0;
    virtual std::optional<int64_t> read_i64(uint64_t addr) const = 0;
};

class process_memory : public memory {
   public:
    bool debug_ = false;

    std::optional<uint8_t> read_u8(uint64_t addr) const override;
    std::optional<uint16_t> read_u16(uint64_t addr) const override;
    std::optional<uint32_t> read_u32(uint64_t addr) const override;
    std::optional<uint64_t> read_u64(uint64_t addr) const override;

    std::optional<int8_t> read_i8(uint64_t addr) const override;
    std::optional<int16_t> read_i16(uint64_t addr) const override;
    std::optional<int32_t> read_i32(uint64_t addr) const override;
    std::optional<int64_t> read_i64(uint64_t addr) const override;
};

} /* namespace libresolver */

#endif /* LIBRESOLVER_INCLUDE_MEMORY_HPP */