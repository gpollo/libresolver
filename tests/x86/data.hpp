#ifndef LIBRESOLVER_TESTS_X86_DATA_HPP
#define LIBRESOLVER_TESTS_X86_DATA_HPP

#include <functional>
#include <vector>
#include <memory>

#include <capstone/capstone.h>

// 48 63 44 85 00          movslq 0x0(%rbp,%rax,4),%rax

namespace tests::x86::data {

static std::vector<std::reference_wrapper<cs_insn>> get_instructions() {
    const unsigned char* bytes = (unsigned char*)
        /* example indirect jump computation */
        "\x83\xff\x06"                  /* #0  cmp $0x6,%edi */
        "\x77\x76"                      /* #1  ja 123 */
        "\x48\x8d\x0d\x0c\x10\x00\x00"  /* #2  lea 0x100c(%rip),%rcx */
        "\x89\xff"                      /* #3  mov %edi,%edi */
        "\x48\x89\xf0"                  /* #4  mov %rsi,%rax */
        "\x48\x63\x14\xb9"              /* #5  movslq (%rcx,%rdi,4),%rdx */
        "\x48\x01\xca"                  /* #6  add %rcx,%rdx */
        "\xff\xe2"                      /* #7  jmpq *%rdx */
        /* other instructions used for testing */
        "\xff\xe0"                      /* #8  jmpq *%rax */
        "\x48\x89\xd8"                  /* #9  mov %rax,%rbx */
        "\x89\xd8"                      /* #10 mov %eax,%ebx */
        "\x48\x8d\x0d\x0c\x12\x00\x00"  /* #11 lea 0x120c(%rip),%rcx */
        "\x48\x8d\x88\x0c\x12\x00\x00"; /* #12 lea 0x120c(%rax),%rcx */
    const size_t bytes_len = 47;

    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
        return {};
    }

    if (cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON) != CS_ERR_OK) {
        return {};
    }

    if (cs_option(handle, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT) != CS_ERR_OK) {
        return {};
    }

    cs_insn* insns;
    const size_t insn_count = 13;
    if (cs_disasm(handle, bytes, bytes_len, 2000, 0, &insns) != insn_count) {
        return {};
    }

    std::vector<std::reference_wrapper<cs_insn>> insn_vec;
    for (size_t i = 0; i < insn_count; i++) {
        insn_vec.push_back(insns[i]);
    }

    return insn_vec;
}

} /* namespace tests::x86::data */

#endif /* LIBRESOLVER_TESTS_X86_DATA_HPP */