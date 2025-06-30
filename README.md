# C-Transpiler-to-ASM

A simple C-to-Assembly transpiler that converts a subset of C-style syntax (variable assignments, arithmetic expressions, `if`, `for`, and `while` statements) into x86 assembly-like instructions.

## Table of Contents
- [Features](#features)
- [Requirements](#requirements)
- [Building](#building)
- [Usage](#usage)
- [Supported Syntax](#supported-syntax)
- [Examples](#examples)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License](#license)

## Features
- Transpiles simple expressions and assignments to assembly operations:
  - `MOV`, `ADD`, `SUB`, `AND`, `OR`, `XOR`, `SHL`, `SHR`, `MUL`, `DIV`
- Supports control flow constructs:
  - `if` statements
  - `for` loops
  - `while` loops
- Registers are chosen based on variable names (`a` → `eax`, `b` → `ebx`, etc.)
- Generates loop labels and jump instructions automatically.

## Requirements
- GCC (or any C compiler) for building the transpiler
- Unix-like environment (Linux, macOS)

## Building
Compile the transpiler using MAKEFILE:

```sh
make
