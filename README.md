# libresolver

The purpose of this library is to resolve the target
addresses of an indirect jump in the purpose of dynamic
binary instrumentation.

## Purpose

There exist multiple dynamic tracing methods. One of them
is to replace, or patch, the instruction while the program
is running. This is usually done by replacing one or more
instructions by a jump that change the flow of the program
to execute instrumentation code. A relevant implementation
of such instrumentation technique exist in the Linux kernel
under the name of [Kprobes](https://www.kernel.org/doc/Documentation/kprobes.txt).

For optimised x86 Kprobes, where instructions are replaced by
a jump and not a trap, it may be necessary to replace
multiple instructions to fit a 5-bytes jump. In that case, it
is crucial that the second or third instruction replaced are
not executed directly. Kprobe analyses the instructions at
the function-level to make sure that no jump lands in those instructions.

There exist two kind of jumps: direct (immediate) jump or
indirect (register or memory) jump. Checking where a direct
jump (e.g. `jmpq 0x142`) lands can be done simply by looking
at the instruction. This is much harder for the indirect jump (e.g. `jmpq %rax`) case. Just by looking at the
instruction, it may jump anywhere in the entire address
space of the program.

Kprobe doesn't overwrite multiple instructions if an indirect
jump is present in the function since it can't tell where it
is going to land.

_The goal of this work is to offer a reliable and efficient
way of computing where an indirect jump may land so that
dynamic tracing application may use optimal probe even if
an indirect jump is found._

For more informations, you can visit the following
documentation files:

* [Source of indirect jump](doc/indirect-jump.md).
* [Description of the algorithm](doc/algorithm.md).
* [Limitations of the algorithm](doc/limitations.md).
* [Possible improvements to the algorithm](doc/improvements.md).

## Dependencies

The library requires a compiler that supports C++20. With
minimal work, it should be possible to downgrade this
requirement to C++17 or even C++14 if needed.

The project is built with the
[Meson](https://mesonbuild.com/) build system.

The following libraries are required:
* [Capstone](https://www.capstone-engine.org/)
* [Keystone](https://www.keystone-engine.org/) (only for tests)

## Build
To build the library, create a build directory. The project
is then configure using `meson` and built with `ninja`.

```
$ mkdir build && cd build
$ meson ..
$ ninja
```

After configuration, tests can be executed using the
following command.

```
$ ninja check
```

## Subproject

This project can be easily included in another Meson
project. You can read more about Meson subprojects
[here](https://mesonbuild.com/Subprojects.html).

The easiest way is to create a
[Wrap file](https://mesonbuild.com/Wrap-dependency-system-manual.html)
named `libresolver.wrap` in your `subprojects/` directory
containing the following.

```ini
[wrap-git]
url = https://github.com/gpollo/libresolver.git
revision = head
```

The subproject can then be imported in your `meson.build`
file using the following declarations.

```meson
libresolver_project = subproject('libresolver')
libresolver_dep = libresolver_project.get_variable('libresolver_dep')
```

Finally, it can be linked with your executable or library
using declarations similar to the following.

```meson
my_executable = executable(
    …,
    dependencies: […, libresolver_dep, …],
    …,
)
```