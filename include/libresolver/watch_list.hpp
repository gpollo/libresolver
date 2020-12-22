#ifndef LIBRESOLVER_INCLUDE_WATCH_LIST_HPP
#define LIBRESOLVER_INCLUDE_WATCH_LIST_HPP

#include <unordered_set>

#include <capstone/capstone.h>

namespace libresolver {

class watch_list {
   public:
    void reset();
    bool matches(cs_insn& insn) const;
    bool add_register(x86_reg reg);
    bool remove_register(x86_reg reg);
    void add_instruction(x86_insn reg);
    void remove_instruction(x86_insn reg);

   private:
    std::unordered_set<x86_reg> registers_;
    std::unordered_set<x86_insn> instructions_;
};

} /* namespace libresolver */

#endif /* LIBRESOLVER_INCLUDE_WATCH_LIST_HPP */