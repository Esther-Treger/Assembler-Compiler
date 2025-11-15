/*-------first assembler run: gathers any information it can about the code (instructions, data, labels) to later be used in the second phase
everything aside from labels is encoded in this run------ */

/* 
Description: 
This function reads an assembly file line by line and creates linked lists of instructions and data while also parsing labels and label attributes. It returns a file pointer to the .ob file that will be used for the second run.
Parameters:
FILE *am: a pointer to the assembly file that is being read.
char *file_name: the name of the assembly file.
struct list_node *instructions: a pointer to the head of the linked list of instructions.
struct list_node *data: a pointer to the head of the linked list of data.
struct hash_table *labels: a pointer to the hash table of labels.
struct hash_table *label_attributes: a pointer to the hash table of label attributes.
Returns: 
a file pointer to the .ob file if no errors were encountered during parsing, or NULL otherwise. */
FILE *first_run(FILE *, char *, struct list_node *, struct list_node *, struct hash_table *, struct hash_table *);

/*
Description: 
This function updates the memory addresses of data labels in a hash table by adding the instruction_count value to their current address, and storing the updated address in the same hash table.
Parameters:
labels: A pointer to a hash table that contains the symbol table for the assembly program.
label_attributes: A pointer to a hash table that contains attributes of the symbols in the labels hash table.
instruction_count: An integer value representing the number of instructions in the program.
Returns:
This function does not return a value, but it modifies the labels hash table by updating the memory addresses of data labels.
*/
void update_data_labels_with_instruction_count(struct hash_table *labels, struct hash_table *label_attributes, int instruction_count);

/*
Description: 
This function checks whether a given input line from an assembly source file is too long. If the line is longer than the maximum allowed length, the function prints an error message and sets a flag to indicate that errors exist in the file.
Parameters:
line: A pointer to a character array representing the input line to check.
am: A pointer to a file stream representing the input assembly file.
file_name: A pointer to a character array representing the name of the input assembly file.
line_count: An integer value representing the current line number in the input assembly file.
errors_exist: A pointer to a Boolean value that is set to True if errors are encountered during the assembly process.
Returns: 
This function returns a Boolean value indicating whether the input line is too long (True) or not (False). If the input line is too long, the function prints an error message and sets the errors_exist flag to True.*/
Bool handle_line_too_long(char *line, FILE *am, char *file_name, int line_count, Bool *errors_exist);

/*
Description: 
This function handles the definition of a label in an assembly source file. It extracts the label name from the input line and performs validation checks to ensure that the label name is valid and not the same as a built-in constant. The function also sets a flag to indicate that a label has been defined.
Parameters:
line: A pointer to a character array representing the input line to handle.
i: A pointer to an integer representing the current position in the input line.
count: An integer representing the length of the label name in the input line.
file_name: A pointer to a character array representing the name of the input assembly file.
line_count: An integer value representing the current line number in the input assembly file.
label_name: A pointer to a character array that will be assigned the label name extracted from the input line.
label_def: A pointer to an integer that will be set to 1 to indicate that a label has been defined.
errors_exist: A pointer to a Boolean value that is set to True if errors are encountered during the assembly process.
Returns: 
This function returns an integer value representing the length of the non-white space characters in the input line after the label definition.*/
int handle_label_definition(char *line, int *i, int count, char *file_name, int line_count, char **label_name, int *label_def, Bool *errors_exist);







