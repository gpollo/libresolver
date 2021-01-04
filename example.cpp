#include <functional>
#include <initializer_list>
#include <optional>
#include <unordered_set>
#include <vector>

#include <capstone/x86.h>

#if 0

expression get_basic_1() {
    /*
     * cmp    $k3,%r3d
     * ja     k2
     * mov    %r3d,%r1d
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * DONE
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::reg(r3_.dword()), addr::reg(r1_.dword())),
            ja(addr::imm(k2_),
            cmp(addr::imm(k3_), addr::reg(r3_.dword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32(i + 4 * (k1_ + rip_[0])) + i;
        }
    };
}

expression get_basic_2() {
    /*
     * cmp    $k3,%r3d
     * ja     k2
     * lea    k1(%rip),%r2
     * mov    %r3d,%r3d
     * movslq (%r2,%r3,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * DONE
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            mov(addr::reg(r3_.dword()), addr::reg(r3_.dword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            ja(addr::imm(k2_),
            cmp(addr::imm(k3_), addr::reg(r3_.dword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_3() {
    /*
     * cmpl   $k4,k5(%rsp)
     * ja     k3
     * mov    k2(%rsp),%r1d
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * DONE
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::base2(k2_, rsp_.qword()), addr::reg(r1_.dword())),
            ja(addr::imm(k3_),
            cmpl(addr::imm(k4_), addr::base2(k5_, rsp_.dword())),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k2_ + rip_[0]) + 4 * i) + (k2_ + rip_[0]);
        }
    };
}

expression get_basic_4() {
    /*
     * cmp    $k3,%r3d
     * ja     k2
     * lea    k1(%rip),%r2
     * movslq (%r2,%r3,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * DONE
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            ja(addr::imm(k2_),
            cmp(addr::imm(k3_), addr::reg(r3_.dword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_5() {
    /*
     * cmp    $k3,%r4d
     * ja     k2
     * lea    k1(%rip),%r2
     * mov    %r4d,%r3d
     * movslq (%r2,%r3,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     *
     * DONE
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            mov(addr::reg(r5.dword()), addr::reg(r3.dword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            ja(addr::imm(k2_),
            cmp(addr::imm(k3_), addr::reg(r4_.dword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_6() {
    /*
     * In this case, the final expression don't depends on the comparison
     * itself because %r1d it depends on a global variable that is reloaded
     * after the comparison.
     *
     *   cmp    $k4,%r1d
     *   ja     k3
     * > mov    k2(%rip),%r1d
     *   lea    k1(%rip),%r2
     *   movslq (%r2,%r1,4),%r1
     *   add    %r2,%r1
     *   jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::base2(k2_, rip_.qword()), addr::reg(r1.dword())),
            ja(addr::imm(k3_),
            cmp(addr::imm(k4_), addr::reg(r1_.dword())),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_7() {
    /*
     * lea    k4(%rip),%r2
     * cmpl   $k3,k1(%r1)
     * ja     k2
     * mov    k1(%r1),%r1d
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            mov(addr::base2(k1_, r1_.qword()), addr::reg(r1_.dword())),
            ja(addr::imm(k2_),
            cmpl(addr::imm(k3_), addr::base2(k1_, r1_.qword())),
            ignore(r1_),
            lea(addr::base2(k4_, rip_.qword()), addr::reg(r2_)),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_8() {
    /*
    * lea    k3(%rip),%r2
    * cmp    $k2,%r1b
    * ja     k1
    * movzbl %r1b,%r1d
    * movslq (%r2,%r1,4),%r1
    * add    %r2,%r1
    * jmpq   *%r1
    */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            movslq(addr::base2(k1_, r1_.byte()), addr::reg(r1_.dword())),
            ja(addr::imm(k1_),
            cmp(addr::imm(k2_), addr::base2(k1_, r1_.byte())),
            ignore(r1_),
            lea(addr::base2(k3_, rip_.qword()), addr::reg(r2_)),
        },
        [&]() {
            return {0, k2_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k3_ + rip_[0]) + 4 * zero_extend_byte(i)) + (k3_ + rip_[0]);
        }
    };
}

expression get_basic_9() {
    /*
     * cmpl   $k4,k5(%rip)
     * ja     k3
     * mov    k2(%rip),%r1d
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.byte()), addr::reg(r2_.qword())),
            mov(addr::base2(k2_, rip_), addr::reg(r1_.dword)),
            ja(addr::imm(k3_),
            cmpl(addr::imm(k4_), addr::base2(k5_, rip_.byte())),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_10() {
    /*
     * cmpl   $k4,k2(%r3)
     * ja     k3
     * mov    k2(%r3),%r1d
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::base2(k2_, r3_.qword()), addr::reg(r1_.dword())),
            ja(addr::imm(k3_),
            cmpl(addr::imm(k4_), addr::base2(k2_, r3_.qword())),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_11() {
    /*
     * In this case, there is no variable index and jump can only land
     * in a single location. The code source that can generate such case
     * is unknown.
     *
     * mov    $k2,%r4d
     * lea    k1(%rip),%r2
     * mov    %r4d,%r3d
     * movslq (%r2,%r3,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            mov(addr::reg(r4_.dword()), addr::reg(r3_.dword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::imm(k2_), addr::reg(r4_.dword())),
        },
        [&]() {
            return {0, 1};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * k2_) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_12() {
    /*
     * lea    k4(%rip),%r2
     * cmpl   $k3,k1(%r3)
     * ja     k2
     * mov    k1(%r3),%r1d
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            mov(addr::base(k1_, r3_.qword()), addr::reg(r1_.dword())),
            ja(addr::imm(k2_)),
            cmpl(addr::imm(k3_), addr::base2(k2_, r3_.qword())),
            ignore(r3_),
            lea(addr::base2(k4_, rip_.qword()), addr::reg(r2_.qword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k4_ + rip_[0]) + 4 * i) + (k4_ + rip_[0]);
        }
    };
}

expression get_basic_13() {
    /*
     * cmp    $k3,%r1d
     * ja     k2
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base(k1_, rip_.qword()), addr::reg(r2_.qword())),
            ja(addr::imm(k2_)),
            cmp(addr::imm(k3_), addr::reg(r1_.dword())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k4_ + rip_[0]) + 4 * i) + (k4_ + rip_[0]);
        }
    };
}

expression get_basic_14() {
    /*
     * cmp    $k3,%r1b
     * ja     k2
     * lea    k1(%rip),%r2
     * movzbl %r1b,%r1d
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            movzbl(addr::reg(r1_.byte()), addr::reg(r1_.dword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            ja(addr::imm(k2_)),
            cmp(addr::imm(k3_), addr::reg(r1_.byte())),
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * zero_extend_byte(i)) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_15() {
    /*
     * cmpl   $k4,k2(%r1)
     * ja     k3
     * mov    k2(%r1),%r1d
     * lea    k1(%rip),%r2
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            lea(addr::base2(k1_, rip_.qword()), addr::reg(r2_.qword())),
            mov(addr::base2(k2_, r1_.qword()), addr::reg(r1_.dword())),
            ja(addr::imm(k3_)),
            cmpl(addr::imm(k4_), addr::base2(k2_, r1_.qword())),
        },
        [&]() {
            return {0, k4_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

expression get_basic_16() {
    /*
     * lea    k3(%rip),%r2
     * cmp    $k2,%r1d
     * ja     k1
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            ja(addr::imm(k1_)),
            cmpl(addr::imm(k2_), addr::reg(r1_.dword())),
            ignore(r1_),
            lea(addr::base2(k3_, rip_.qword()), addr::reg(r2_.qword())),
        },
        [&]() {
            return {0, k2_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k3_ + rip_[0]) + 4 * i) + (k3_ + rip_[0]);
        }
    };
}

expression get_basic_17() {
    /*
     * lea k3(%rip),%r2
     * cmp $k2,%r3b
     * ja k1
     * movslq (%r2,%r3,4),%r1
     * add %r2,%r1
     * jmpq *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            ja(addr::imm(k1_)),
            cmp(addr::imm(k2_), addr::reg(r3_.byte())),
            ignore(r3_),
            lea(addr::base2(k3_, rip_.qword()), addr::reg(r2_.qword())),
        },
        [&]() {
            return {0, k2_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k3_ + rip_[0]) + 4 * i) + (k3_ + rip_[0]);
        }
    };
}

expression get_basic_18() {
    /*
     * lea    k3(%rip),%r2
     * cmp    $k2,%r1d
     * ja     k1
     * mov    %r1d,%r1d
     * movslq (%r2,%r1,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r1_.qword(), 4), addr::reg(r1_.qword())),
            ja(addr::imm(k1_)),
            cmp(addr::imm(k2_), addr::reg(r1_.dword())),
            ignore(r1_),
            lea(addr::base2(k3_, rip_.qword()), addr::reg(r2_.qword())),
        },
        [&]() {
            return {0, k2_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k3_ + rip_[0]) + 4 * i) + (k3_ + rip_[0]);
        }
    };
}

expression get_basic_19() {
    /*
     * cmp    $k3,%r4b
     * ja     k2
     * lea    k1(%rip),%r2
     * movzbl %r4b,%r3d
     * movslq (%r2,%r3,4),%r1
     * add    %r2,%r1
     * jmpq   *%r1
     */
    return {
        {
            jmpq(addr::reg(r1_.qword())),
            add(addr::reg(r2_.qword()), addr::reg(r1_.qword())),
            movslq(addr::base4(0, r2_.qword(), r3_.qword(), 4), addr::reg(r1_.qword())),
            movzbl(addr::reg(r4.byte()), addr::reg(r3.dword())),
            lea(addr::base2(k1_, rip_.qword())),
            ja(addr::imm(k2_)),
            cmp(addr::imm(k3_), addr::reg(r4_.byte()))
            
        },
        [&]() {
            return {0, k3_};
        },
        [&](int i) {
            return memory_.read_sign_extend_32((k1_ + rip_[0]) + 4 * i) + (k1_ + rip_[0]);
        }
    };
}

/* 
 * # Algorithm
 * 
 * process first instruction (adds a register to the watch list)
 * while (pattern not matched && no error):
 *     find next instruction that affects a register in the watch list
 *     generate hash of this instruction
 *     if we can move forward in the expression tree
 *         move foward in the expression tree
 *             a register might get added or removed in the watch list
 *         if we are at a leaf
 *             set pattern matched
 *     else
 *         set error
 *
 * ## Simple Example
 *
 * ```
 * cmp    $0x20,%eax
 * ja     0x200
 * lea    0x100(%rip),%rbx
 * mov    0x1d12e2(%rip),%rcx
 * movslq (%rbx,%rax,4),%rax
 * add    %rbx,%rax
 * jmpq   *%rax
 * ```
 * 
 * 1. `process(jmpq *%rax)`
 *   - `%r1 = %rax` (allocated)
 *   - `move_forward(hash(jmpq *%r1))`
 *   - `watch_list.add(%rax)`
 * 2. `instructions.find_next(watch_list)`
 * 3. `process(add %rbx,%rax)`
 *   - `%r1 = %rax` (reused)
 *   - `%r2 = %rbx` (allocated)
 *   - `move_forward(hash(add %r2,%r1))`
 *   - `watch_list.add(%rbx)`
 * 4. `instructions.find_next(watch_list)`
 * 5. `process(movslq (%rbx,%rax,4),%rax)`
 *   - `%r1 = %rax` (reused)
 *   - `%r2 = %rbx` (reused)
 *   - `move_forward(hash(movslq (%r2,%r1,4),%r1))`
 * 6. `instructions.find_next(watch_list)`
 * 7. `process(movslq (%rbx,%rax,4),%rax)`
 *   - `%r1 = %rax` (reused)
 *   - `%r2 = %rbx` (reused)
 *   - `move_forward(hash(movslq (%r2,%r1,4),%r1))`
 * 8. `instructions.find_next(watch_list)`
 *   - `mov 0x1d12e2(%rip),%rcx` is skipped
 * 9. `process(lea 0x100(%rip),%rbx)`
 *   - `k1 = 0x100` (allocated)
 *   - `%r2 = %rbx` (reused)
 *   - `move_forward(hash(lea k1(%rip),%r2))`
 *   - `watch_list.remove(%rbx)`
 * 10. `instructions.find_next(watch_list)`
 * 11. `process(lea 0x100(%rip),%rbx)`
 *   - `k1 = 0x100` (allocated)
 *   - `%r2 = %rbx` (reused)
 *   - `move_forward(hash(lea k1(%rip),%r2))`
 *   - `watch_list.remove(%rbx)`
 *   - `watch_list.add(ja)`
 * 12. `instructions.find_next(watch_list)`
 * 13. `process(ja 0x200)`
 *   - `k2 = 0x200` (allocated)
 *   - `move_forward(hash(ja k2))`
 *   - `watch_list.remove(ja)`
 * 14. `instructions.find_next(watch_list)`
 * 15. `process(cmp $0x20,%eax)`
 *   - `k3 = 0x20` (allocated)
 *   - `%r1d = %eax` (reused)
 *   - `move_forward(hash(cmp $k1,%r1d))`
 *   - `watch_list.remove(%r1)`
 * 16. leaf reached, pattern matched
 */

/*
 * Instruction rewinding should skip instructions that
 * don't affect already used registers. There should be
 * the concept of a register watch list. After a instruction
 * has been matched, the instruction should be able to
 * remove a specific register from the watch list. Each
 * time a new register is encountered, it is added to the
 * watch list.
 *
 *   cmp    $k3,%r4h
 *   ja     k2
 *   lea    k1(%rip),%r2
 *   mov    %r4h,%r3h
 * > mov    %r1h,%r12d
 *   movslq (%r2,%r3,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   cmpl   $k4,k2(%r3)
 * > mov    0x3c(%r3),%ebp
 *   ja     k3
 *   mov    k2(%r3),%r1d
 *   lea    k1(%rip),%r2
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   cmpl   $k3,k2(%r3)
 * > mov    0xc(%r3),%ebx
 * > movq   $0x0,0x10(%rsp)
 * > movq   $0x0,0x18(%rsp)
 * > mov    %r1,0x20(%rsp)
 *   ja     1395ec
 *   mov    k2(%r3),%r1d
 *   lea    k1(%rip),%r2
 * > mov    %rdi,%rbp
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   cmp    $k3,%r4
 *   ja     k2
 *   lea    k1(%rip),%r2
 * > mov    %r3,%rdi
 *   mov    %r4,%r3d
 *   movslq (%r2,%r3,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   cmp    $k3,%r1d
 *   ja     k2
 *   lea    k1(%rip),%r2
 * > mov    0x1d12e2(%rip),%rcx
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   cmpl   $k4,(k2%r2)
 * > mov    %r1,0x128(%r9)
 * > mov    %r1,0x78(%r9)
 *   ja     k3
 *   mov    k2(%r2),%r1d
 *   lea    k1(%rip),%r2
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * It should also skip instructions on registers where the
 * bound has already been set.
 *
 *   lea    k5(%rip),%r2
 * > nopw   0x0(%r1,%r1,1)
 * > mov    k4(%rsp),%r3
 * > lea    (%r4,%r4,2),%r1
 * > lea    (%r3,%r1,8),%r1
 *   cmpl   $k3,k1(%r1)
 *   ja     k2
 *   mov    k1(%r1),%r1d
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 *   lea    k4(%rip),%r2
 * > movq   $0x0,0x98(%rsp)
 * > mov    %r1,0xa0(%rsp)
 * > test   %r3,%r3
 * > je     c103a
 * > mov    %r12,0x18(%rsp)
 * > nopl   0x0(%r1)
 *   cmpl   $k3,k1(%r3)
 *   ja     k2
 *   mov    k1(%r3),%r1d
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * It should also be a good idea to check if other direct jumps
 * land in the switch case pattern as they can be long.
 *
 *   lea    0x22e374(%rip),%r2
 * > test   %r1d,%r1d
 * > jle    52673
 * > nopl   0x0(%r1)
 * > mov    (%r12),%r1
 * > lea    0x0(,%rbp,8),%rdx
 * > mov    (%r1,%rbp,8),%r1
 * > movsbl 0x12(%r1),%esi
 * > mov    (%r1),%r15
 * > mov    0x8(%r1),%rbx
 * > lea    -0x41(%rsi),%r1d
 *   cmp    $k2,%r1b
 *   ja     k1
 *   movzbl %r1b,%r1d
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * The base pointer should be treated like a normal register.
 * This example would be covered by existing cases if %rbp is
 * is considered as %r3.
 *
 * > cmp    $k3,%ebp
 *   ja     k2
 *   lea    k1(%rip),%r2
 * > movslq (%r2,%rbp,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * This case is handled if the (%r3) is interpreted as k2(%r3),
 * where k2=0. That said, `cmpl $k3,(%r3)` and `cmpl $k3,0(%r3)`
 * are two differents instructions (the later being 1 byte longer).
 *
 * > cmpl   $k3,(%r3)
 *   ja     k2
 * > mov    (%r3),%r1d
 *   lea    k1(%rip),%r2
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * This test case might be handled if we search for instruction
 * not just above, but also at the source of other jumps that
 * might land here. Because of the `retq` instruction, `%r2`
 * is not resolved.
 *
 *   retq
 * > nopl   (%rax)
 * > movsbl 0x1(%rbx),%ecx
 * > lea    0x2(%rbx),%r15
 * > lea    -0x22(%rcx),%r1d
 * > mov    %ecx,%eax
 *   cmp    $k2,%r1b
 *   ja     k1
 *   movzbl %r1b,%r1d
 *   movslq 0x0(%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 *
 * Same thing for `%r14`.
 *
 *   retq   
 * > xchg   %ax,%ax
 *   cmp    $0x5,%bl
 *   ja     1c6e04
 *   movslq (%r14,%rbx,4),%rsi
 *   add    %r14,%rsi
 *   jmpq   *%rsi*
 *
 * In this example, there is a function call in the middle of
 * the jump computation and `%r2` is computed before. The problem
 * is that the function call might clobber the register. In order
 * for this case to be supported, the instructions in the function
 * should be analysed.
 *
 *   lea    k3(%rip),%r2
 * > mov    %r12,%rdi
 * > addr32 callq 1abe10
 *   cmp    $k2,%r1d
 *   ja     k1
 *   mov    %r1d,%r1d
 *   movslq (%r2,%r1,4),%r1
 *   add    %r2,%r1
 *   jmpq   *%r1
 */

/*


jmpq *%r1
    add %r2,%r1
        movslq (%r2,%r3,4),%r1
        |   mov %r3d,%r3d
        |   |   lea k1(%rip),%r2
        |   |       ja k2
        |   |           cmp $k3,%r3d
        |   mov %r4d,%r3d
        |   |   lea k1(%rip),%r2
        |   |       ja k2
        |   |       |   cmp $k3,%r4d
        |   |       mov $k2,%r4d
        |   lea k1(%rip),%r2
        |   |   ja k2
        |   |       cmp $k3,%r3d
        |   ja k1
        |   |   cmp $k2,%r3b
        |   |       lea k3(%rip),%r2
        |   movzbl %r4b,%r3d
        |       lea k1(%rip),%r2
        |           ja k2
        |               cmp $k3,%r4b
        movslq (%r2,%r1,4),%r1
            lea k1(%rip),%r2
            |   mov %r3d,%r1d
            |   |   ja k2
            |   |       cmp $k3,%r3d
            |   mov k2(%r1),%r1d [index is in a struct]
            |   |   ja k3
            |   |       cmpl $k4,k2(%r1)
            |   mov k2(%rsp),%r1d [index is on the stack, same as previous if %rsp is a normal register]
            |   |   ja k3
            |   |       cmpl $k4,k5(%rsp)
            |   mov k2(%rip),%r1d [index is in a global variable, same as previous if %rip is a normal register]
            |   |   ja k3
            |   |       cmpl $k4,k5(%rip)
            |   |       cmp $k4,%r1d
            |   mov k2(%r3),%r1d
            |   |   ja k3
            |   |       cmpl $k4,k2(%r3)
            |   ja k2
            |       cmp $k3,%r1d
            mov k1(%r1),%r1d
            |   ja k2
            |       cmpl $k3,k1(%r1) [ignore r1]
            |           lea k4(%rip),%r2
            movzbl %r1b,%r1d
            |   ja k1
            |   |   cmp $k2,%r1b [ignore r1]
            |   |       lea k3(%rip),%r2
            |   lea k1(%rip),%r2
            |       ja k2
            |           cmp $k3,%r1b
            mov k1(%r3),%r1d
            |   ja k2
            |       cmpl $k3,k1(%r3)
            |           lea k4(%rip),%r2
            ja k1
            |   cmp $k2,%r1d
            |       lea k3(%rip),%r2
            mov %r1d,%r1d
                ja k1
                    cmp $k2,%r1d
                        lea k3(%rip),%r2
   




   

jmpq
    *%r1
        add
            %r2,
                %r1
                    movslq
                        (%r2,
                            %r3,
                            |   4),
                            |       %r1
                            |           mov
                            |           |   %r3d,
                            |           |   |   %r3d
                            |           |   |       lea 
                            |           |   |           k1(
                            |           |   |               %rip),
                            |           |   |                   %r2
                            |           |   |                       ja
                            |           |   |                           k2
                            |           |   |                               cmp
                            |           |   |                                   $k3,
                            |           |   |                                       %r3d
                            |           |   %r4d,
                            |           |       %r3d
                            |           |           lea
                            |           |               k1(
                            |           |                   %rip),
                            |           |                       %r2
                            |           |                           ja
                            |           |                               k2
                            |           |                                   cmp
                            |           |                                       $k3,
                            |           |                                           %r4d
                            |           lea
                            |               k1(
                            |                   %rip),
                            |                       %r2
                            |                           ja
                            |                               k2
                            |                                   cmp
                            |                                       $k3,
                            |                                           %r3d
                            %r1,
                                4),
                                    %r1
                                        lea
                                            k1(
                                                %rip),
                                                    %r2
                                                        mov
                                                            %r3d,
                                                            |   %r1d
                                                            |       ja
                                                            |           k2
                                                            |               cmp
                                                            |                   $k3,
                                                            |                       %r3d
                                                            k2(
                                                                %rsp),
                                                                |   %r1d
                                                                |       ja
                                                                |           k3
                                                                |               cmpl
                                                                |                   $k4,
                                                                |                       k5(
                                                                |                           %rsp)
                                                                %rip),
                                                                    %r1d
                                                                        ja
                                                                            k3
                                                                                cmp
                                                                                    $k4,
                                                                                        %r1d
*/

} /* namespace insn */

/*
    /usr/bin/git: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV),
    dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2,
    BuildID[sha1]=c80967f9e4baecd8b1ff4284ab7f1ff5b53057af, for GNU/Linux 3.2.0, stripped

    from official git 2.29.2-1 package of Arch Linux

    mov    0x2fc652(%rip),%eax
    lea    0x30(%rsp),%r13
    movl   $0x0,0x2fc643(%rip)
    mov    %eax,0xc(%rsp)
    nopl   (%rax)
    mov    0x303151(%rip),%rsi
    mov    %r13,%rdi
    addr32 callq 219420
    cmp    $0xffffffff,%eax
    je     330b0
    mov    0x1a4(%rsp),%r10d
    mov    0x40(%rsp),%r14
    test   %r10d,%r10d
    je     32c67
    lea    0x28194c(%rip),%rsi
    mov    %r14,%rdi
    callq  *0x2f10c5(%rip)
    mov    %rax,%r15
    test   %rax,%rax
    jne    33038
    mov    %r15,0x178(%rsp)
    mov    0x74(%rsp),%r9d
    xor    %edx,%edx
    mov    %r12,%rcx
    mov    %r14,%rsi
    mov    0x343e0d(%rip),%rdi
    lea    0xd0(%rsp),%r8
    test   %r9d,%r9d
    setne  %dl
    shl    $0x6,%edx
    addr32 callq 212fd0
    test   %eax,%eax
    je     33010 # if %eax == 0 then jmp
  > lea    0x6(%rax),%edx
    cmp    $0xfffffffa,%eax
    jb     3312c # if 0x6(%rax) > 0xfffffffa then jmp
  > lea    0x243f67(%rip),%rcx
  > movslq (%rcx,%rdx,4),%rdx
  > add    %rcx,%rdx
  > jmpq   *%rdx

rcx rdx
ecx edx
eax

rdx
rdx + rcx
[rcx + 4 * rdx] + rcx
[rcx + 4 * (0x6 + eax)] + rcx
*/

/*
 * These indirect jumps are unlikely to be recoverable as they are
 * the result of calling a function. They are probably not switch
 * cases.
 *
 *    addr32 callq 2494c0
 * >  nopl   0x0(%rax)
 *    mov    0x10(%rax),%rax
 *    mov    %r10d,%esi
 *    mov    %r9,%rdi
 *    jmpq   *%rax
 *
 *    callq  1c9610
 * >  mov    %rbp,%rsi
 * >  pop    %rbp
 * >  mov    %rax,%rdi
 *    mov    (%rax),%rax
 *    mov    0x18(%rax),%rax
 *    jmpq   *%rax

 */

#endif