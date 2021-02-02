#ifndef LIBRESOLVER_TESTS_X86_ENGINE_HPP
#define LIBRESOLVER_TESTS_X86_ENGINE_HPP

#include <vector>
#include <functional>

#include <capstone/capstone.h>
#include <keystone/keystone.h>
#include <libresolver/utils/no_copy.hpp>

namespace tests::x86 {

class engine {
public:
    engine();
    ~engine();

    class decoded : public libresolver::utils::no_copy {
    public:
        decoded(cs_insn& insns, size_t count);
        ~decoded();

        size_t size() const;
        const cs_insn& at(size_t i) const;
        const cs_insn& operator[](size_t i) const;
        std::vector<std::reference_wrapper<cs_insn>> get(bool reverse = true);

    private:
        cs_insn* insns_;
        const size_t count_;
    };

    class encoded : public libresolver::utils::no_copy {
    public:
        encoded(uint8_t& bytes, size_t count);
        ~encoded();

        size_t size() const;
        uint8_t at(size_t i) const;
        uint8_t operator[](size_t i) const;
        const uint8_t* get() const;

    private:
        uint8_t* bytes_;
        size_t count_;
    };

    std::unique_ptr<decoded> disassemble(const unsigned char* bytes, size_t bytes_len, uint64_t addr, size_t expected_count);
    std::unique_ptr<encoded> assemble(const char* assembly, uint64_t addr, size_t expected_instructions);
    std::unique_ptr<decoded> assemble_and_disassemble(const char* assembly, uint64_t addr, size_t expected_statements, size_t expected_instructions);

private:
    csh cs_handle_;
    ks_engine* ks_handle_;
};

} /* namespace tests::x86 */

#endif /* LIBRESOLVER_TESTS_X86_ENGINE_HPP */