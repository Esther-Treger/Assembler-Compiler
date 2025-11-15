/* ----------Functions that serve as utility functions for memory allocation, file handling, and error message management.---------- */


/* `Description:
Allocates memory of size s and returns a pointer to it. Exits the program with an error message if memory allocation fails.
Parameters:
size_t s: The size of memory to allocate.
Returns:
A pointer to the allocated memory. */
void *alloc(size_t);

/* Description:
Opens a file with the given file_name in the specified mode and returns a pointer to the file. Exits the program with an error message if the file cannot be opened.
Parameters:
char *file_name: The name of the file to open.
char *mode: The mode to open the file in.
Returns:
A pointer to the opened file.*/
FILE *open(char *, char *);

/* Description:
Closes the file and returns 0 if successful or exits the program with an error message if it fails.
Parameters:
FILE *file: A pointer to the file to close.
Returns:
0 if successful, otherwise exits the program with an error message.*/
int close(FILE *);

/* Description:
Initializes the message array with error and warning messages.
Parameters:
None
Returns:
None */
void error_util_prepare(void);

/* Description:
Frees the memory allocated for the message array.
Parameters:
None
Returns:
None */
void error_util_free(void);

/* Description:
Prints a warning message for the specified error code e at the given file_name and line. Supports variadic arguments for message formatting.
Parameters:
char *file_name: The name of the file where the warning occurred.
int line: The line number where the warning occurred.
enum err_util_errno e: The error code for the warning message.
Returns:
None*/
void print_warning(char *, int, enum err_util_errno, ...);

/* Description:
Prints an error message for the specified error code e at the given file_name and line. Supports variadic arguments for message formatting.
Parameters:
char *file_name: The name of the file where the error occurred.
int line: The line number where the error occurred.
enum err_util_errno e: The error code for the error message.
Returns:
None */
void print_error(char *, int, enum err_util_errno, ...);

/*
Description:
Prints the final message with the given prefix, file_name, line, and format string s. Supports printf-like formatting using a va_list.
Parameters:
char *prefix: The prefix to add to the message (e.g., "Error" or "Warning").
char *file_name: The name of the file where the message is related to.
int line: The line number where the message is related to.
char *s: The format string for the message.
va_list args: The list of variadic arguments for message formatting.
Returns:
None
*/
void final_print(char *, char *, int, char *, va_list);
