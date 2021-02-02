#include <tests/external/catch.hpp>
#include <tests/x86/engine.hpp>

namespace tests::x86 {

/* engine class */

engine::engine() {
    REQUIRE(cs_open(CS_ARCH_X86, CS_MODE_64, &cs_handle_) == CS_ERR_OK);
    REQUIRE(cs_option(cs_handle_, CS_OPT_DETAIL, CS_OPT_ON) == CS_ERR_OK);
    REQUIRE(cs_option(cs_handle_, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT) == CS_ERR_OK);
    REQUIRE(ks_open(KS_ARCH_X86, KS_MODE_64, &ks_handle_) == KS_ERR_OK);
    REQUIRE(ks_option(ks_handle_, KS_OPT_SYNTAX, KS_OPT_SYNTAX_ATT) == KS_ERR_OK);
}

engine::~engine() {
    REQUIRE(cs_close(&cs_handle_) == CS_ERR_OK);
    REQUIRE(ks_close(ks_handle_) == KS_ERR_OK);
}

std::unique_ptr<engine::decoded> engine::disassemble(const unsigned char* bytes, size_t bytes_len, uint64_t addr,
                                                     size_t expected_count) {
    cs_insn* insns;
    REQUIRE(cs_disasm(cs_handle_, bytes, bytes_len, addr, 0, &insns) == expected_count);

    return std::make_unique<engine::decoded>(*insns, expected_count);
}

std::unique_ptr<engine::encoded> engine::assemble(const char* assembly, uint64_t addr, size_t expected_statements) {
    uint8_t* bytes = nullptr;
    size_t count;
    size_t statements;
    REQUIRE(ks_asm(ks_handle_, assembly, addr, &bytes, &count, &statements) == 0);
    REQUIRE(statements == expected_statements);

    return std::make_unique<engine::encoded>(*bytes, count);
}

std::unique_ptr<engine::decoded> engine::assemble_and_disassemble(const char* assembly, uint64_t addr,
                                                                  size_t expected_statements,
                                                                  size_t expected_instructions) {
    auto encoded = assemble(assembly, addr, expected_statements);
    auto decoded = disassemble(encoded->get(), encoded->size(), addr, expected_instructions);

    return decoded;
}

/* engine::decoded class */

engine::decoded::decoded(cs_insn& insns, size_t count) : insns_(&insns), count_(count) {}

engine::decoded::~decoded() {
    cs_free(insns_, count_);
}

size_t engine::decoded::size() const {
    return count_;
}

const cs_insn& engine::decoded::at(size_t i) const {
    return insns_[i];
}

const cs_insn& engine::decoded::operator[](size_t i) const {
    return at(i);
}

std::vector<std::reference_wrapper<cs_insn>> engine::decoded::get(bool reverse) {
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

/* engine::encoded class */

engine::encoded::encoded(uint8_t& bytes, size_t count) : bytes_(&bytes), count_(count) {}

engine::encoded::~encoded() {
    ks_free(bytes_);
}

size_t engine::encoded::size() const {
    return count_;
}

uint8_t engine::encoded::at(size_t i) const {
    return bytes_[i];
}

uint8_t engine::encoded::operator[](size_t i) const {
    return at(i);
}

const uint8_t* engine::encoded::get() const {
    return bytes_;
}

} /* namespace tests::x86 */