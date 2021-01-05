#ifndef LIBRESOLVER_INCLUDE_PORCELAIN_HPP
#define LIBRESOLVER_INCLUDE_PORCELAIN_HPP

#include <stdint.h>

#include <capstone/capstone.h>

#ifdef __cplusplus
extern "C" {
#endif

int libresolver_x86_resolve(csh handle, cs_insn* insns, size_t insn_count, size_t index, uint64_t** results,
                            void* memory);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LIBRESOLVER_INCLUDE_PORCELAIN_HPP */