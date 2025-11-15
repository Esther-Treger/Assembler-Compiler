/*
A structure representing a node in a hash table. It contains a pointer to the next node in the linked list, a key (a string used to index the node), and a value (a pointer to the data stored in the node).*/
struct hash_node {
	struct hash_node *next;
	char *key;
	void *value;
};

/*
A structure representing a hash table. It contains an array of pointers to hash_node objects, representing the buckets of the table, as well as the size of the table (the number of elements in it).*/
struct hash_table {
	struct hash_node *tab[HASH_TABLE_CAPACITY];
	int size;
};


/* This is a struct used to define a node of a linked list. It contains a pointer to the next node in the list, a pointer to the value stored in this node, and a function pointer to free the memory used by the value when the node is removed from the list.
*/
struct list_node {
	struct list_node *next;
	void *value;
	void (*free)(void *);
};


typedef enum
{
    True = 0,
    False = 1
} Bool;

/*
defines a set of error and warning codes that represent various issues that can arise during the assembly process. 
*/
enum err_util_errno {
	ERROR_LABELINVALIDNAME,
	ERROR_LABELSYMBOL,
	ERROR_LABELDEFINED,
	ERROR_LABELNOTDEFINED,
	WARNING_LABELEMPTY,
	WARNING_LABELUSELESS,

	ERROR_PARAMSUNEXPECTEDSPACE,
	ERROR_PARAMSUNEXPECTEDCOMMA,
	ERROR_PARAMSNOTENOUGH,
	ERROR_PARAMSTOOMANY,
	ERROR_PARAMSJUMP,

	ERROR_OPINVALIDSOURCE,
	ERROR_OPINVALIDDEST,
	ERROR_DATAINVALIDNUMBER,

	ERROR_STRINGSTARTQUOTES,
	ERROR_STRINGUNFINISHED,
	ERROR_STRINGASCII,

	ERROR_UNKNOWNINSTRUCTION,

	ERROR_LINEOVERFLOW,
	ERROR_BINARYOVERFLOW,

	ERRORCOUNT /* amount of err_util_errno values for later use */
};


/* An enumeration of error codes used in the assembler program. Each error code represents a specific type of error that can occur during the parsing and assembly process.*/ 
enum parser_error_codes {
	PARSER_OK,
	PARSER_EUNEXPECTEDSPACE,
	PARSER_EUNEXPECTEDCOMMA,
	PARSER_ENOTENOUGHPARAMS,
	PARSER_ETOOMANYPARAMS,
	PARSER_EEXPECTEDQUOTES,
	PARSER_EUNFINISHEDSTRING,
	PARSER_EINVALIDCHAR,
	PARSER_EJUMPPARAMS,
	PARSER_EINVALIDNUMBER,
	PARSER_EINVALIDLABEL,
	PARSER_EINVALIDDESTPARAM,
	PARSER_EINVALIDSOURCEPARAM
};

typedef struct word {
	unsigned int field : WORD_LENGTH;
} word;

typedef struct instruction {
	void *value;
	int is_label;
	int line; /* the line associated with this instruction (0 if unknown) */
} instruction;

/*Defines constants for all possible symbols that can be encountered in the assembly code. These include operations, directives , registers, and an unknown symbol.
*/
enum symbol {
	UNKNOWN_SYMBOL = -1,

	OPCODE_MOV,
	OPCODE_CMP,
	OPCODE_ADD,
	OPCODE_SUB,
	OPCODE_NOT,
	OPCODE_CLR,
	OPCODE_LEA,
	OPCODE_INC,
	OPCODE_DEC,
	OPCODE_JMP,
	OPCODE_BNE,
	OPCODE_RED,
	OPCODE_PRN,
	OPCODE_JSR,
	OPCODE_RTS,
	OPCODE_STOP,

	DIRECTIVE_DATA,
	DIRECTIVE_STRING,
	DIRECTIVE_ENTRY,
	DIRECTIVE_EXTERN,

	REGISTER
};

/*Defines constants for the three types of encoding that can be used in the assembly code: absolute, external, and relocatable.*/
enum encoding {
	ENC_ABSOLUTE,
	ENC_EXTERNAL, /* externals */
	ENC_RELOCATABLE /* labels */
};

/*
Defines constants for the five types of addressing methods that can be used in the assembly code: error, instant number, direct label, jump with parameters, and direct register.
*/
enum address_method {
	ADDRESS_ERROR = -1,
	ADDRESS_INSTANT, /* instant number */
	ADDRESS_DIRECT, /* label */
	ADDRESS_JUMP_WITH_PARAMS,
	ADDRESS_DIRECT_REGISTER /* register */
};

/*
defines the specific bit positions for various components of an opcode in the assembled binary.
*/
enum op_bit_locations {
	OPBIT_ARE_0,
	OPBIT_ARE_1,
	OPBIT_DEST_2,
	OPBIT_DEST_3,
	OPBIT_SOURCE_4,
	OPBIT_SOURCE_5,
	OPBIT_OPCODE_6,
	OPBIT_OPCODE_7,
	OPBIT_OPCODE_8,
	OPBIT_OPCODE_9,
	OPBIT_PARAM2_10,
	OPBIT_PARAM2_11,
	OPBIT_PARAM1_12,
	OPBIT_PARAM1_13
};

/*
Defines constants for the six possible numbers of parameters that can be associated with an operation or directive in the assembly code: list (for .data), unknown, none, single, two, and jump (for OPCODE_JMP, OPCODE_BNE, and OPCODE_JSR).
*/
enum param_amount {
	PARAM_LIST = -2, /* .data */
	PARAM_UNKNOWN,
	PARAM_NONE,
	PARAM_SINGLE,
	PARAM_TWO,
	PARAM_JUMP
};

/*
Defines bit flags that can be used to store metadata about a label, such as whether it is an entry, an external, stores data or an instruction.
*/
enum label_attribute {
	LABEL_ENTRY = 1 << 0,
	LABEL_EXTERNAL = 1 << 1,
	LABEL_DATA = 1 << 2,
	LABEL_INSTRUCTION = 1 << 3
};
