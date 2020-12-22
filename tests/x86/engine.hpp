#ifndef LIBRESOLVER_TESTS_X86_ENGINE_HPP
#define LIBRESOLVER_TESTS_X86_ENGINE_HPP

#include <vector>
#include <functional>

#include <capstone/capstone.h>
#include <tests/external/catch.hpp>
#include <libresolver/utils/no_copy.hpp>

namespace tests::x86 {

class engine {
public:
    engine() {
        REQUIRE(cs_open(CS_ARCH_X86, CS_MODE_64, &handle_) == CS_ERR_OK);
        REQUIRE(cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON) == CS_ERR_OK);
        REQUIRE(cs_option(handle_, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT) == CS_ERR_OK);
    }

    ~engine() {
        REQUIRE(cs_close(&handle_) == CS_ERR_OK);
    }

    class instructions : libresolver::utils::no_copy {
    public:
        instructions(cs_insn& insns, size_t count) : insns_(&insns), count_(count) {}

        ~instructions() {
            cs_free(insns_, count_);
        }

        size_t size() const {
            return count_;
        }

        const cs_insn& operator[](size_t i) const {
            return insns_[i];
        }

        std::vector<std::reference_wrapper<cs_insn>> get(bool reverse = true) {
            std::vector<std::reference_wrapper<cs_insn>> vector;

            for (size_t i = 0; i < count_; i++) {
                if (reverse) {
                    vector.push_back(insns_[count_ - i - 1]);
                } else {
                    vector.push_back(insns_[i]);
                }
            }

            return vector;
        }

    private:
        cs_insn* insns_;
        const size_t count_;
    };

    std::unique_ptr<instructions> disassemble(const unsigned char* bytes, size_t bytes_len, uint64_t addr, size_t expected_count) {
        cs_insn* insns;
        REQUIRE(cs_disasm(handle_, bytes, bytes_len, addr, 0, &insns) == expected_count);

        return std::make_unique<instructions>(*insns, expected_count);
    }

private:
    csh handle_;
};

} /* namespace tests::x86 */

#endif /* LIBRESOLVER_TESTS_X86_ENGINE_HPP */