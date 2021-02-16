# Limitations

This page describes the limitations of the current
implementation of the algorithm.

## Atomic Sequences

Some sequences of instructions should be considered _atomic_
in the tree.

### Example

When part of the pattern computation gets bounded, it is often
done by the combination of  `ja` followed by a `cmp`. The
following assembly code is a example of a supported pattern.

```
lea    142(%rip),%rbx
cmp    $4,%cl
ja     10
movslq (%rbx,%rcx,4),%rax
add    %rbx,%rax
jmpq   *%rax
```

The way that this pattern is matched is `ja` is added to the
watch list at the `movslq`. Once it is matched, `ja` is
removed and `cmp` is added to the watch list. When `cmp`
is match, it is then removed as well.

It is possible that another `ja`/`cmp`, unrelated to the
pattern, be compiled in the instructions. The following
instructions is an example.

```
lea    142(%rip),%rbx
cmp    $4,%cl
ja     10
cmp    $1423,%rdi
ja     42
movslq (%rbx,%rcx,4),%rax
add    %rbx,%rax
jmpq   *%rax
```

Here, `ja 42` would match, but `cmp $1423,%rdi` would not
and pattern matching would fail. The problem may be worse
in the following example.

```
lea    142(%rip),%rbx
cmp    $4,%cl
ja     10
test   %rdi,%rdi
ja     42
movslq (%rbx,%rcx,4),%rax
add    %rbx,%rax
jmpq   *%rax
```

In this case, `ja 42` would match and also `cmp $4,%cl`. In
fact, pattern matching would work here, but with a invalid
instruction in it.

### Solution

The solution is to treat `ja`/`cmp` sequence as atomic. When
the matcher receives a `ja`, it should find which instruction
modify the corresponding flags. Then, both instructions are
treated atomically in the pattern tree.

In the last example, the first `ja` would be paired with the
`test` directly before. Both of them would be ignored by the
watch list. The second `ja` would be paired with the `cmp`
before and they would match in the tree.

## 32-bit Patterns

Currently, we only support 64-bit x86 pattern.

### Example

The following pseudo-assembly describe a 64-bit pattern
for indirect jump compiled from a switch case jump table.

```
lea    k7(%rip),%r2
cmpl   $k5,k6(%r4)
ja     k4
mov    k3(%r4),%r3
movslq k1(%r2,%r3,k2),%r1
add    %r2,%r1
jmp    *%r1
```

The `mov` is a 64-bit memory read and the `movslq` is a 32-bit
memory read. The equivalent pseudo-assembly pattern for a
32-bit system is shown below.

```
call k8
add  $k7,%r2
cmpl $k5,k6(%r4)
ja   k4
mov  k3(%r4),%r3
mov  k1(%r2,%r3,k2),%r1
add  %r2,%r1
jmp  *%r1
```

Notice that the `movslq` is replace by a `mov`. Both `mov`
are 32-bit memory read. The `lea` is also replaced by a
`add` followed by a `call`.

Under 64-bit systems, the address of the jump table is
computed using the `lea`. Under 32-bit systems, the address
is fetch from the [_global offset table_](https://en.wikipedia.org/wiki/Global_Offset_Table) (GOT). The memory
address of the GOT is computed at the start of each function
with a call to a function. With debugging symbols, this
function will look something like:

* `__x86.get_pc_thunk.bx`
* `__x86.get_pc_thunk.dx`
* `__x86.get_pc_thunk.si`
* `__x86.get_pc_thunk.bp`
* and more

In order to reliably match 32-bit patterns, we would need to
check that the `call` instruction goes to one of these
function. It currently cannot be done since the algorithm
does not take as input the symbol table. 

### Solution

Add the symbol table as input to the algorithm and these
pattern should be reliable.

## Multiple Paths

Some switch-case pattern looks odd at first sight, for
example, the one below.

```
mov    $0x1,%ecx
lea    0x1cb7e6(%rip),%rsi
mov    %ecx,%edx
movslq (%rsi,%rdx,4),%rax
add    %rsi,%rax
jmpq   *%rax
```

By looking only at this pattern, there is only one target
address for the indirect jump. That is because there is
another jump in the function that target on instruction
in the pattern. It may look like this.

```
    mov    $0x1,%ecx
DO_JUMP:
    lea    0x1cb7e6(%rip),%rsi
    mov    %ecx,%edx
    movslq (%rsi,%rdx,4),%rax
    add    %rsi,%rax
    jmpq   *%rax
SOMEWHERE_ELSE:
    cmpl   $0x6,%ecx
    ja     0x1582db
    jmpq   DO_JUMP
```

The value of `%ecx` may be directly set to 1 or it may get
bounded between 0 to 6 inclusive based on which branch it
takes. Multiple path code is not supported by the current
implementation of the library.

### Solution

When matching instructions, it may be possible to have
multiplie path. All path target addresses should be computed
and merged together as the final result.

## Tail Optimized Indirect Call

One source of indirect jump are function call that are tail
optimized. In those cases, the `call` instruction is replaced
by a `jmq` instruction.

If it is a indirect function call, such as a function pointer
or a virtual method, the indirect `call` will be replaced
by an indirect `jmq`.

### Solution

There is currently no solution based on pattern matching for
resolving or knowing that an indirect jump is a function
call.

The following pieces of assembly show some example of
indirect call that are tail-optimized into a indirect jump.

```
pop rbx
pop rbp
pop r12
pop r13
jmp rcx
```

```
pop rbx
pop rbp
pop r12
pop r13
pop r14
pop r15
jmp rax
```