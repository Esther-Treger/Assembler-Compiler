🛠️ Assembler Compiler – Two-Pass Assembler (C, Linux)

This project implements a two-pass assembler in C, capable of parsing assembly source files, handling macros, resolving symbols, encoding instructions, and generating the required output files:

.ob – machine code

.ent – entry symbols

.ext – external symbols

The assembler is organized into two main phases:

First Pass – parse code, collect symbols, macros, and data.

Second Pass – resolve symbol usage, validate references, and generate output files.

📌 General Rules

Whitespace includes spaces " " and tabs \t.

Lines are separated by \n.

Input consists of .as files containing instructions, labels, macros, and data definitions.

📚 Data Structures Used
Hash Table

A custom hash table is implemented to store:

Program labels

Macro names and their contents

Built-in constants and metadata

It supports storing both integer and string values to handle different memory allocation needs.

Linked List

A linked-list structure is used to preserve the order of:

Instructions

Data declarations (.data, .string)

Each node includes a free function pointer to correctly release memory.

⚙️ Utility Modules
error_util

Wrapper functions for operations that may fail

Consistent error/warning printing

str_util

String manipulation helpers used across the assembler

symbols

Symbol table management

Handles built-in constants, label attributes, entries, and externals

🧩 Macro System

Macros are parsed before assembly begins.

Rules:

Macros follow this structure:

mcr <macro_name>
<macro content>
endmcr


Macro names cannot match built-in constants.

Invalid macro definitions are ignored.

Macro tokens must match exactly (e.g., m1 is valid, but m1: is not).

🔄 First Pass (Analysis)

The assembler performs:

Macro expansion

Instruction parsing

Label collection

Data encoding

Creation of symbol attributes (data/code/entry/external)

All code and data elements are stored in the appropriate linked lists or hash tables.

🔄 Second Pass (Encoding)

In the second pass the assembler:

Validates label usage

Resolves symbol references

Encodes instructions into machine format

Generates:

.ob file (machine code)

.ent file (if entry labels exist)

.ext file (if external labels exist)

If any label is undefined, output files are not generated.

📁 Output Files
File	Description
file.ob	Encoded machine code
file.ent	List of entry labels
file.ext	List of external references
