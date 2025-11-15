/* MAMMAN 14 ESTHER TREGER 213836083 AND REUT SHILONI 325418333  */

Welcome to our assembler implementation! Our implementation is split into two main phases - the first phase, in which we gather information about the code, and the second phase, in which we encode the gathered information and output it into .ob, .ent, and .ext files.

#General Assumptions
We use spaces ( ) and tabs (\t) as whitespace, and \n as newlines.

#Data Structures
Our implementation uses a hash table and a linked list for storing and organizing data.

#Hash Table
We've implemented our own hash table, which we use to store Macro Layout macros, program labels, and built-in constants. Our hash table has functions for getting/setting values of strings and integers to address the different ways that memory should be allocated for them.

#Linked List
We use a linked list to save the program instructions and data in the order in which they appear. Our linked list node implementation contains a free field which is used when freeing the list, to be able to correctly free the value.

#Utilities
We have several utility modules in our implementation, including:

error_util, which contains error message utilities. It has wrappers for functions that may fail, and utility functions for printing errors and warnings.
str_util, which contains functions for string manipulations.
symbols, which stores the symbols/built-in constants registry, and contains utility functions related to them.

#Macro Layout
Our Macro Layout implementation parses macros without any error checking. Invalid macro definitions will result in the definition not being read. Macros will only be parsed after they are defined.

The Macro Layout treats macro names as "tokens". For example, if we define a macro called m1, writing m1 will be valid but m1: will not be considered as a macro.

#Macro Definition Structure
Macro names cannot be the same as any built-in constants. If a built-in constant is used as a name, its definition will be read but the macro will not be parsed. The legal structure of macro definitions is as follows:

asm
Copy code
mcr <macro name>
<macro content>
endmcr

#First Run
In the first phase of our implementation, we gather information about the code, such as instructions, data, labels, and label attributes. Instructions are stored in a linked list, and contain operation names and their parameters in the order that they appear in the program. Data is a linked list of .data and .string definitions, which are already encoded. Labels are stored in a hash table, which contains label names and their values. A label will only have a value when defined. Label attributes are also stored in a hash table, and contain metadata about labels, such as whether a label stores data or an operation, and whether a label is an entry or an external.

#Second Run
In the second phase of our implementation, we use the information gathered in the first phase to check for the validity of label usage. If all labels used are defined, the encoding is printed into the .ob file. If any entries are defined, an .ent file is created, and if any externals are defined, an .ext file is created.
