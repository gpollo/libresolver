## Indirect Jump

The most common programming patterns that generate indirect
jump are switch-case statement and tail-optimised function
call.

### Switch-Case Statement

Modern compilers may compile a switch-case statement into
different kind of assembly pattern. For simple switch-case
statement with few cases, it is usually simpler to interpret
the switch-case as a bunch of `if` statements.

When there is a lot of cases with successive case value, the
compiler will generate a jump table. For example, let's
assume the following C code.

```c
switch (i) {
    case 0:  /* do something */ break;
    case 1:  /* do something */ break;
    case 2:  /* do something */ break;
    case 3:  /* do something */ break;
    case 4:  /* do something */ break;
    case 5:  /* do something */ break;
    case 6:  /* do something */ break;
    case 7:  /* do something */ break;
    default: /* do something */ break;
}
```

There is 8 cases and all values are successive. It may get
compiled into something that looks like the following
assembly code (`i` would be in `%eax`). Notice that there
is an indirect jump generated.

```nasm
cmp    eax,7               ; check if case is supported
ja     0x812               ; else, jump into default case
lea    rbx,[0x1362 + rip]  ; get address of the jump table
movsxd rax,[rbx + rax * 4] ; get jump offset from jump table
add    rax,rbx             ; compute final jump
jmp    *rax                ; jump into case
```

When there is a lot of case values and they are sparse, the
compiler will generate, depending on optimisation, a binary
search tree. They are less efficient than jump table, but
require less memory. They aren't problematic because they
don't generate indirect jump.

### Tail-Optimised Function Call

The following simple C code calls a sub-function. It will
most likely get compiled into a simple `call` instruction.

```C
void bar(void);

void foo(void) {
    /* stuff */
    bar();
    /* stuff */
}
```

If the sub-function is called at the end of the current
function, shown below, the compiler may choose to generate
a direct `jmp` instruction.

```C
void bar(void);

void foo(void) {
    /* stuff */
    bar();
}
```

The compiler is allowed to do so, because the current
function is done using the current function frame and it may
be reused by sub-function. This is called [tail-call](https://en.wikipedia.org/wiki/Tail_call)
optimisation and it is very helpful for recursive functions.
It may allow them to never run out of stack.

The problem comes when the direct function call is replaced
by an indirect function call, as shown below.

```C
void foo(void (*bar)(void)) {
    /* stuff */
    bar();
}
```

With no optimisation, the compiler will generate an indirect
`call`. With optimisation, it will generate an indirect
`jmp`. C++ will generate even more indirect jumps because
`virtual` function are equivalent to indirect function call.

_Currently, this library does not resolve indirect jump from
tail-call optimised indirect call._