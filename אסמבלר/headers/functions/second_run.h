/*------- two functions that together make up the second pass of the assembler process in the given code. second_run takes in various parameters and uses them to encode the assembly code into binary format, outputting it into a binary file, as well as creating the .ent and .ext files. encode is called within second_run to encode each number as it is processed.--------*/


/* Parameters:
	FILE *am: a pointer to the input assembly file
	FILE *ob: a pointer to the output binary file
	char *file_name: the name of the assembly file
	struct list_node *instructions: a pointer to the head of the list of 		instruction nodes
	struct list_node *data: a pointer to the head of the list of data nodes
	struct hash_table *labels: a pointer to the hash table containing the labels and their values
	struct hash_table *label_attributes: a pointer to the hash table containing the labels and their attributes
Purpose: 
	executes the second pass of the assembler process, encoding the assembly code into binary format and outputting it into a binary file (.ob), as well as creating the .ent and .ext files
Returns: an integer indicating whether errors were found during execution (0 if no errors, 1 otherwise) */
Bool second_run(FILE *am, FILE *ob, char *file_name, struct list_node *instructions, struct list_node *data, struct hash_table *labels, struct hash_table *label_attributes);

/* Parameters:
	unsigned int num: the number to be encoded
	FILE *ob: a pointer to the output binary file
Purpose:
	encodes a number into the special binary format (././) and outputs it into the binary file */
void encode(unsigned int num, FILE *ob);




