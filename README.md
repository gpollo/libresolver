# libresolver

## TODO

* Some sequences of instructions should be considered _atomic_
  in the tree. For example, the `ja` followed by a `cmp`.
* Does register size really matter? Do we need a pattern for
  byte, word, double-words and quad-words? Or a catch-all pattern
  is good enough?
* We use AT&T syntax. Make sure the code base follows that.
* The instruction `cltq` sign-extend %eax into %rax. Check
  if there is equivalent instructions for other register.
  If so, they should match the same instruction in the
  pattern tree.
* When rewinding instructions, we need to go through possible
  jump source. For example, some instructions of a pattern are
  match in a block A, then jump to a block B where the rest of
  the instructions are.
    * See `cmd_replace @ git/builtin/replace.c`.
    * What happens if there is multiple jump sources?
* Sometime, we are done using a matched register. This register,
  for example %rax, should reused as another placeholder register.
* All memory reads are assumed to be on a 64-bit system.
    * This can be problematic for non position-independent executable
      where the target address is stored directly in the jump table.
      On 64-bit systems, each entries are 8 bytes. On 32-bit systems,
      each entries are 4 bytes. Right now, we assume entries that are
      8 bytes long.

## Tail Optimized Indirect Call

```
94763500460964: jne 0x562fd8c85b70
94763500460966: add rsp, 8
94763500460970: mov rdx, r13
94763500460973: mov rsi, rbp
94763500460976: pop rbx
94763500460977: pop rbp
94763500460978: pop r12
94763500460980: pop r13
94763500460982: jmp rcx
```

```
93912806519545: pop rbx
93912806519546: pop rbp
93912806519547: pop r12
93912806519549: pop r13
93912806519551: pop r14
93912806519553: pop r15
93912806519555: jmp rax
```