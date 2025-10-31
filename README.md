## ARC Toolchain - Overview

This project aims to provide a ***complete language toolchain*** — from language specification, to compiler, to bytecode
and a virtual-machine runtime (in the spirit of Java) — but with a syntax closer to C.
However: as of now, this repository implements only the primitive **bytecode instruction set**, a minimal **runtime
virtual machine**, and a minimal **AST node set**. It does not yet include a *lexer*, *parser*, *full compiler
front-end*, *language specification*, or any *optimisations*. The work is undertaken as a **personal learning project
** (to build experience and showcase capability) rather than as a production-ready toolchain.

*In other words*: you are looking at the *early skeleton of a toolchain*. The goal is to evolve it gradually into
something more complete. Use it, study it, extend it, but keep in mind its current limitations.

## Implemented Features

* A basic bytecode format to represent operations and control flow.
* A minimal virtual machine that can load and interpret the bytecode.
* A simple AST node set – supporting minimal operations and constructs in the runtime.
* The project is structured in C++ and serves as an *educational and demonstrative engine for how a language toolchain
  might be built*.

## Non-features / Known limitations

* No lexer implemented (tokeniser).
* No parser implemented (no front-end that converts source code to AST yet).
* No complete compiler that generates bytecode from user-written source.
* No formal language specification (grammar, semantics) published yet.
* No optimisation passes, no advanced compiler features.
* Not intended (yet) for production use, deployment, or general-purpose development.

## Build Instructions

*Note*: Only Windows and Linux-based distributions are currently supported.

**1.** Ensure the following perequisite tools are installed:
<div style="margin-left: 2em">

- A *C++20* (or later) compliant **compiler** (*clang* is the only one tested against).
- **CMake** (minimum version 3.20).
- Standard build tools (*Ninja* is the only one tested against).

</div>

**2.** Inside the root directory, configure the project using *CMake* by executing the following command:
<div style="margin-left: 2em">

```
cmake -S . -B build/Debug -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

</div>

**3.** Inside the root directory, build the project by executing the following command:
<div style="margin-left: 2em">

```
cmake --build build/Debug
```

</div>

This process (if successfully) will generate a an executable named ***arc*** inside the *build/Debug* directory (or
*build/Release* if that is the configuration you are building) that you can just run.

## Next Steps & Roadmap

* Define a *language specification*.
* Implement a *lexer* for the C-like syntax.
* Implement a *parser* that produces *AST nodes*.
* Implement a *compiler front-end* that traverses the AST and emits *bytecode* using the defined instruction set.
* Improve the VM: *native function calls*, heap allocations, and most likely implement *garbage collection*.
* Add *optimisations* to the compiler (constant folding, dead-code elimination, etc.).
* Design and publish a bytecode specification (e.g., binary/in-memory encoding) and convert the virtual machine and
  compiler front-end to generate that binary bytecode instead.
* Add *examples and integration tests*: write simple programs in your language, compile them, run on the VM, document
  behaviour.
* Polish the tooling: command-line compiler driver, bytecode dump utility, debugger integration.
