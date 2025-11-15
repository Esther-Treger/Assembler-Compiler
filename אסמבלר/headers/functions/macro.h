/* functions that are related to the parsing and handling of macros in assembly code.*/


/* Description: 
The macro_layout function reads an assembly file line by line and writes a new file with the same code but with macro definitions expanded. It searches the assembly file for macro definitions and expands them when encountered in the code.
Parameters:
FILE *as: A pointer to the assembly file to be read.
char *file_name: A string containing the name of the file to write the expanded code to.
Returns:
 A pointer to the opened file with the expanded code.*/
FILE *macro_layout(FILE *fd, char *nameF);

/* Description:
 Checks if a string is a valid macro definition.
Parameters:
char *line: A pointer to the string to be checked.
int *i: A pointer to the current index in the string.
int *count: A pointer to the number of characters in the string.
char **macro_name: A pointer to a string that will store the macro name if it is found.
Returns: 
1 if the string is a valid macro definition, 0 otherwise. */
int is_valid_mcr(char *, int *, int *, char **);

/*Description: 
Checks if a string is a valid end macro definition.
Parameters:
char *line: A pointer to the string to be checked.
int *i: A pointer to the current index in the string.
int *count: A pointer to the number of characters in the string.
Returns: 
1 if the string is a valid end macro definition, 0 otherwise.*/
int is_valid_end_mcr(char *, int *, int *);

/* Description: 
Allocates memory for storing the content of a macro.
Parameters:
FILE *as: A pointer to the assembly file containing the macro content.
Returns:
 A pointer to the allocated memory.

 */
char *macro_content_alloc(FILE *);


