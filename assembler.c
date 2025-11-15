#include "data.h"

int main(int argc, char **argv)
{

	/*Call handle_source_files function passing the command line arguments that are the files.*/
	handle_source_files(argc, argv);
	return 0;

}

int handle_source_files(int argc, char **argv)
{
	int i;
	Bool file_executed_successfully=True;
	char *file_name;

	if (argc < 2) {
		fprintf(ERROR_OUTPUT, "error: no files sent\n");
		exit(1);
	}
	
	/* Prepare for symbol handling and error handling. */
	symbols_prepare();
	error_util_prepare();

	for (i = 1; i < argc; i++) {
		file_name = alloc(sizeof(char) * (strlen(argv[i]) + 4 + 1));
		strcpy(file_name, argv[i]);
		strcat(file_name, AS_SUFFIX);

		file_executed_successfully= handle_single_file(file_name);
		if(file_executed_successfully)
		{
			fprintf(ERROR_OUTPUT, "-----------------------------\n");
			fprintf(ERROR_OUTPUT, "file %s was executed successfully\n",file_name);
			fprintf(ERROR_OUTPUT, "-----------------------------\n");
		}
		else
		{
			fprintf(ERROR_OUTPUT, "-----------------------------\n");
			fprintf(ERROR_OUTPUT, "file %s was not executed successfully\n",file_name);
			fprintf(ERROR_OUTPUT, "-----------------------------\n");
		}
		free(file_name);
	}

	/* Free memory used for symbols and error handling. */
	symbols_free();
	error_util_free();

	return 0;
}

Bool handle_single_file(char *file_name) 
{
	FILE *source_file, *macro_layout_file, *object_file;
	struct hash_table *symbol_table, *attribute_table;
	struct list_node *instructions, *data;
	Bool file_executed_successfully=False;
	
	/* Open source file, generate macro layout and close source file. */
	source_file = open(file_name, "r");
	replace_extension(file_name, EMPTY_STRING);
	macro_layout_file = macro_layout(source_file, file_name);
	close(source_file);

	/* Initialize symbol and attribute hash tables, instruction and data lists. */
	symbol_table = hash_table_new();
	attribute_table = hash_table_new();
	instructions = linked_list_new_node(NULL, free);
	data = linked_list_new_node(NULL, free);
	
	/* First run to generate the object file. */
	object_file = first_run(macro_layout_file, file_name, instructions, data, symbol_table, attribute_table);

	/* Reset macro_layout_file pointer to the start and run the second pass to resolve symbols and generate the object file. */
	fseek(macro_layout_file, 0, SEEK_SET);

	if ((object_file == NULL || (second_run(macro_layout_file, object_file, file_name, instructions, data, symbol_table, attribute_table))==True) && close(macro_layout_file) != EOF)
	{

		/* Clean up generated files and mark file as executed successfully. */
		strcat(file_name, ENT_SUFFIX);
		remove(file_name);
		replace_extension(file_name, EXT_SUFFIX);
		remove(file_name);
		replace_extension(file_name, OB_SUFFIX);
		remove(file_name);
		file_executed_successfully=True;
	}

	/* Free memory used for symbols, attributes, instructions and data. */
	hash_table_free(symbol_table);
	hash_table_free(attribute_table);
	linked_list_free(instructions);
	linked_list_free(data);

	return file_executed_successfully;
}
