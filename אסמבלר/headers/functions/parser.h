/* ------These functions are used to parse and encode assembly language instructions and data into machine code instructions and data, using a linked list data structure. Together, they allow for the full assembly and encoding process to be performed on an input assembly file, producing a machine code output file.-------- */


/* Description: Encodes a label value as a relocatable value.
Parameters:
int val: The value of the label to be encoded.
Returns: An unsigned integer encoding the given label value.*/
unsigned int encode_label(int);

/*
Description: Encodes an operation and its parameters and appends them to the list of instructions.
Parameters:
char *op_name: A string containing the name of the operation to be encoded.
enum symbol OPCODE: An enum value representing the opcode of the operation to be encoded.
int line: The line number where the operation was found in the input file.
struct list_node **params: A pointer to the linked list of parameters for the operation.
struct list_node **instruction_pointer: A pointer to a pointer to the current end of the linked list of instructions.
int *instruction_count: A pointer to the current count of instructions in the linked list.
Returns: An error code representing the success or failure of the encoding process.*/
enum parser_error_codes encode_operation(char *op_name, enum symbol OPCODE, int line, struct list_node **params, struct list_node **instruction_pointer, int *instruction_count);

/* Description: 
Encodes a number and appends it to the list of data.
Parameters:
char *s: A string containing the number to be encoded.
struct list_node **data_pointer: A pointer to a pointer to the current 	end of the linked list of data.
int *data_count: A pointer to the current count of data in the linked list.
Returns: 
1 if the number was successfully encoded and added to the list, 0 otherwise. */
int encode_num(char *, struct list_node **, int *);

/*
Description: 
Encodes a string and appends it to the list of data.
Parameters:
char *s: A string containing the text to be encoded.
struct list_node **data_pointer: A pointer to a pointer to the current end of the linked list of data.
int *data_count: A pointer to the current count of data in the linked list.
Returns: 
PARSER_OK if the string was successfully encoded and added to the list, an error code otherwise.
*/
enum parser_error_codes encode_string(char *s, struct list_node **data_pointerr, int *data_count);

/* Description:
Parses a line of assembly code and extracts the parameters.
Parameters:
char *line: The line of assembly code to be parsed.
int *i: A pointer to the current position in the line.
int param_amount: The expected number of parameters. Use PARAM_UNKNOWN for unknown number of parameters, PARAM_SINGLE for a single parameter, and PARAM_JUMP for a jump instruction (which may have one or more parameters enclosed in parentheses).
struct list_node **head: A pointer to a pointer to the head of the linked list of parameters.
Returns: 
PARSER_OK if the parameters were successfully parsed and added to the list, an error code otherwise.*/
enum parser_error_codes parse_params(char *, int *, int, struct list_node **);

/*Description: 
Determines the addressing method of an operand.
Parameters:
char *s: A string containing the operand.
Returns: 
ADDRESS_INSTANT if the operand is an instant address, ADDRESS_DIRECT_REGISTER if the operand is a direct register, ADDRESS_DIRECT if the operand is a direct address, ADDRESS_ERROR otherwise.*/
enum address_method determine_address_method(char *s);

/* Description: Determines whether a character represents a valid space character or not. (either ' ' or '\t')
Parameters:
char c: The character to be checked.
Returns: 1 if the character represents a valid space character, 0 otherwise.*/
int is_valid_space(char c);

/*  Description: Determines whether a character represents a line terminator or not, (either '\n', '\0', or EOF)
Parameters:
char c: The character to be checked.
Returns: 1 if the character represents a line terminator, 0 otherwise. */
int is_line_terminator(char c);

/* Description: Determines whether a string represents a valid number or not.
Parameters:
char *s: The string to be checked.
Returns: 1 if the string represents a valid number, 0 otherwise. */
int is_valid_num(char *s);

/* Description: Determines whether a string represents a valid label or not.
A valid label can start with a letter and can be followed by any number of letters or digits, and its length must not exceed 30 characters. Also, the label must not already exist in the symbol table.
Parameters:
char *s: The string to be checked.
Returns: 1 if the string represents a valid label, 0 otherwise. */
int is_valid_label(char *s);

/*Description: Skips over white space characters in a string starting from a given index.
Parameters:
char line[]: The string to be skipped over.
int *i: A pointer to the current index in the string to start skipping from.
Returns: The number of white space characters that were skipped. */
int skip_white_space(char line[], int * i);

/* Description: Counts the number of non-white space characters in a string starting from a given index.
Parameters:
char line[]: The string to be counted.
int *i: A pointer to the current index in the string to start counting from.
Returns: The number of non-white space characters in the string. */
int count_non_white_space(char line[], int *i);

/*Description: Counts the number of characters in a string until a given character or line terminator is reached.
Parameters:
char *s: The string to be counted.
char c: The character or line terminator to stop counting at.
Returns: The number of characters in the string up to and including the given character or line terminator. */
int count_until(char *s, char c);

/* Description: Creates a new string containing the characters of the input string up to and including a given character or line terminator.
Parameters:
char *s: The string to copy.
char c: The character or line terminator to stop copying at.
Returns: A new string containing the characters of the input string up to and including the given character or line terminator. */
char *dupl_until(char *s, char c);

