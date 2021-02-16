# Improvements

## Reusing Registers

Sometime, we are done using a matched register because its
value get set by a constant or is bounded. In that case, even
if, for example, `%rax` is assigned to `%r2`, it should be
reassignable to another placeholder, like `%r3`, if `%rax`
gets ignored later.

## Register Extensions

The instruction `cltq` sign-extend `%eax` into `%rax`. As
far as I known, there is no equivalent for other register.
That said, it can be done with other instructions. For
example, `%ebx` can be sign-extended by the instruction
`movslq %ebx,%rbx`. In the pattern tree, register extension
instructions should be represented by the same virtual
instruction. This would allow us to remove duplicate cases.
