# Implementation of Pass-1 and Pass-2 Assembler
A complete implementation of a **Two-Pass Assembler** in C++ that converts assembly language source code into machine code through two distinct passes.
## Overview
This assembler implements the classic two-pass algorithm used in systems programming:
- **Pass 1:** Analyzes the source code, builds symbol and literal tables, and generates intermediate code.
- **Pass 2:** Resolves all symbolic references and generates final machine code.

## Features
### Supported Instructions
**Imperative Statements (`IS`)**

- **Arithmetic:** `ADD`, `SUB`, `MULT`, `DIV`
- **Data Movement:** `MOVER`, `MOVEM`
- **Comparison**: `COMP`
- **Control Flow:** `BC` (Branch Conditional), `STOP`
- **I/O Operations:** `READ`, `PRINT`

**Declarative Statements (`DL`)**

- `DC` - Declare Constant
- `DS` - Declare Storage

**Assembler Directives (`AD`)**

- `START` - Set starting address
- `END` - Mark end of program
- `ORIGIN` - Change location counter
- `EQU` - Define symbolic constants
- `LTORG` - Force literal allocation

## Project Structure
```
two-pass-assembler/
├── main.cpp              # Main driver program
├── pass1.h/.cpp          # Pass 1 implementation
├── pass2.h/.cpp          # Pass 2 implementation
├── tokenizer.h/.cpp      # Assembly line tokenizer
├── symbol_table.h/.cpp   # Symbol table management
├── literal_table.h/.cpp  # Literal table management
├── mnemonic_table.h/.cpp # Instruction definitions
├── Makefile             # Build configuration
├── input.asm            # Sample input file
└── README.md            # This file
```

## Architecture
```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│  Tokenizer  │    │ Symbol Table│    │Literal Table│
└─────────────┘    └─────────────┘    └─────────────┘
        │                   │                   │
        └───────────────────┼───────────────────┘
                            │
                    ┌─────────────┐
                    │   Pass 1    │
                    └─────────────┘
                            │
                    ┌─────────────┐
                    │ Intermediate│
                    │    Code     │
                    └─────────────┘
                            │
                    ┌─────────────┐
                    │   Pass 2    │
                    └─────────────┘
                            │
                    ┌─────────────┐
                    │ Machine Code│
                    └─────────────┘
```
## Build Instructions
**1. Clone the repository:**
```
git clone https://github.com/altaccPran/TE-ASSIGNMENTS.git
cd TE-ASSIGNMENTS
```
**2. Compile using Make:**
```
make
```
**3. Clean build files and run:**
```
make clean
/.main
```
