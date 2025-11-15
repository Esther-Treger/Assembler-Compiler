/*-----The handle_source_files and handle_single_file functions work together to perform a two-pass assembly process on a set of assembly files. handle_source_files manages the iteration over the command-line arguments, calling handle_single_file on each file to perform the actual assembly process. handle_single_file performs the two passes on a single assembly file, generating object files, entry files, and external files as necessary. Memory allocation and deallocation for data structures used in the assembly process is managed by both functions. Together, these functions provide a complete assembly process for a set of assembly files.------*/


/*
Description:
Processes the given assembly files one by one, performing a two-pass assembly process to generate object files, entry files, and external files. Manages memory allocation and deallocation for data structures used throughout the assembly process.
Parameters:
int argc: The number of command-line arguments.*
char **argv: An array of pointers to the command-line arguments, containing the file names to process.
Returns:
0 on successful execution, otherwise exits with an error message if no files are provided as arguments.*/
int handle_source_files(int argc, char *argv[]);

/*
Description:
Performs a two-pass assembly process on a single assembly file, generating object, entry, and external files. Manages memory allocation and deallocation for data structures used throughout the assembly process.
Parameters:
char *file_name: The name of the assembly file to process.
Returns:
true on successful execution, otherwise false.
*/
Bool handle_single_file(char *file_name);
