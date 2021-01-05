#include <cstdlib>
#include <functional>
#include <limits>

#include <libresolver/memory.hpp>
#include <tests/external/catch.hpp>

namespace tests::memory {

template <typename T, unsigned int N>
static void test_memory_accesses(std::function<std::optional<T>(uint64_t)> read_memory) {
    std::array<T*, N> values;

    for (unsigned int i = 0; i < N; i++) {
        values[i] = new T((((double)std::rand()) / ((double)RAND_MAX + 1)) * std::numeric_limits<T>::max());
    }

    for (unsigned int i = 0; i < 10; i++) {
        auto got_opt = read_memory((uint64_t)values[i]);
        REQUIRE(got_opt.has_value());
        REQUIRE(got_opt.value() == *values[i]);
    }

    for (unsigned int i = 0; i < N; i++) {
        delete values[i];
    }
}

TEST_CASE("reading from process memory should work", "[libresolver::memory]") {
    libresolver::process_memory memory;

    SECTION("reading uint8_t should work") {
        test_memory_accesses<uint8_t, 10>(
            std::bind(&libresolver::process_memory::read_u8, memory, std::placeholders::_1));
    }

    SECTION("reading uin16_t should work") {
        test_memory_accesses<uint16_t, 10>(
            std::bind(&libresolver::process_memory::read_u16, memory, std::placeholders::_1));
    }

    SECTION("reading uint32_t should work") {
        test_memory_accesses<uint32_t, 10>(
            std::bind(&libresolver::process_memory::read_u32, memory, std::placeholders::_1));
    }

    SECTION("reading uint64_t should work") {
        test_memory_accesses<uint64_t, 10>(
            std::bind(&libresolver::process_memory::read_u64, memory, std::placeholders::_1));
    }

    SECTION("reading int8_t should work") {
        test_memory_accesses<int8_t, 10>(
            std::bind(&libresolver::process_memory::read_i8, memory, std::placeholders::_1));
    }

    SECTION("reading in16_t should work") {
        test_memory_accesses<int16_t, 10>(
            std::bind(&libresolver::process_memory::read_i16, memory, std::placeholders::_1));
    }

    SECTION("reading int32_t should work") {
        test_memory_accesses<int32_t, 10>(
            std::bind(&libresolver::process_memory::read_i32, memory, std::placeholders::_1));
    }

    SECTION("reading int64_t should work") {
        test_memory_accesses<int64_t, 10>(
            std::bind(&libresolver::process_memory::read_i64, memory, std::placeholders::_1));
    }
}

} /* namespace tests::memory */