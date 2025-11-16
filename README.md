🧩 Assembler Compiler — Two-Pass Assembler (C, Linux)

A full two-pass assembler implemented in C as part of the Systems Programming Laboratory (20465) course at the Open University of Israel.
The project includes macro expansion, symbol resolution, instruction encoding, and machine-code generation—designed to closely simulate the behavior of real assembler tools.

📘 Overview

This assembler processes a custom assembly language and produces the required output files through two main phases:

1️⃣ First Pass

Parse each line and validate syntax

Identify labels and assign addresses

Process .data and .string directives

Build symbol and attribute tables

Collect information needed for encoding

2️⃣ Second Pass

Resolve symbol references

Encode instructions into machine code

Generate the following output files:

.ob — machine code

.ent — entry symbols

.ext — external references

The assembler uses custom implementations of:

Hash Table — symbol table, macros, attributes

Linked List — ordered instruction & data storage

Utility Modules — string handling, error tracking, and parsing helpers

📂 Project Structure
assembler.c        – Main driver of the assembler
first_run.c        – First pass: parsing & symbol table creation
second_run.c       – Second pass: encoding & output generation
macro.c            – Macro expansion logic
parser.c           – Line parsing, tokenizing, validation
hash_table.c       – Custom hash table implementation
linked_list.c      – Linked list for instructions/data
symbols.c          – Symbol and attribute handlers
errors.c           – Error logging utilities
utils.c            – General helper functions
makefile           – Build script

Headers/           – Header files (.h)
Tests_and_results/ – Input & output samples

⚙️ Build & Run Instructions
Requirements

GCC compiler

Linux or compatible environment

Build
make

Run
./assembler file1.as file2.as ...


For each source file, the assembler outputs:

file.ob

file.ent (if entry symbols exist)

file.ext (if externs exist)

🧪 Testing

The Tests_and_results directory includes ready-to-use examples demonstrating:

Various addressing modes

Macro handling

Syntax errors & validation

Expected output file formats

📝 Notes

Designed with modularity, clarity, and data abstraction in mind

Includes documented functions and commented logic throughout

Educational project — not intended for production deployment
