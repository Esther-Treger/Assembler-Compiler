/* --------------functions that are related to the management and manipulation of the symbol registry for the assembler.--------------*/


/* Initializes the hash tables and parameter amount definitions for the symbol registry. */
void symbols_prepare(void);

/* Description: Frees the memory allocated for the symbol registry.
 */
void symbols_free(void);

/* Description: Returns the symbol type (operation, directive, or register) for a given string symbol name.
Parameters:
op: the symbol name as a string
Returns:
the symbol type as an enum (either an operation, directive, register, or unknown) */
enum symbol symbols_get(char *);

/*Description: Returns the expected number of parameters for a given symbol type.
Parameters:
op: the symbol type as an enum
Returns:
the expected number of parameters as an enum (none, single, two, list, jump, or unknown)*/
enum param_amount symbols_get_param_amount(enum symbol);

/* Description: Determines if a given string symbol name is an operation.
Parameters:
s: the symbol name as a string
Returns:
1 if s is an operation, otherwise 0 */
int is_operation(char *);

/**Description: Determines if a given string symbol name is a directive.
Parameters:
s: the symbol name as a string
Returns:
1 if s is a directive, otherwise 0*/
int is_directive(char *);

/*Description: Determines if a given string symbol name is a register.
Parameters:
s: the symbol name as a string
Returns:
1 if s is a register, otherwise */
int is_register(char *);

/*Description: Determines if a given symbol type is a jump operation.
Parameters:
o: the symbol type as an enum
Returns:
1 if o is a jump operation, otherwise 0*/
int is_jump_operation(enum symbol);

/* Description: Determines if a given symbol type is a data directive.
Parameters:
o: the symbol type as an enum
Returns:
1 if o is a data directive, otherwise 0 */
int is_data_directive(enum symbol);
