#include "data.h"

FILE *first_run(FILE *am, char *file_name, struct list_node *instructions, struct list_node *data, struct hash_table *labels, struct hash_table *label_attributes)
{
	char line[MAX_LINE_LENGTH + NEWLINE_AND_NULL], /* The current line being processed */
	*label_name, /* The label name in a label definition, if present */
	*op_name; /* The operation name in the current line */
	int i, /* The current character being processed in the line */
	count, /* Results from the `skip_white_space` and `count_non_white_space` functions */
	is_label_def, /* Whether the current line is a label definition */
	param_amount, /* The number of parameters in the current operation */
	*label_attribute, /* Hash table values for label attributes */
	line_count = 0, /* The total number of lines processed */
	data_count = 0, /* The total number of data instructions encountered */
	instruction_count = 0; /* The total number of instructions encountered */
	Bool line_too_long = False; /* Whether the current line is too long */
	Bool errors_exist = False; /* Whether an error has been detected in the file */
	enum symbol op_code; /* The operation code in the current line */
	struct list_node *params, /* The parameters of the current operation */
	*instruction_pointer = instructions, /* A pointer to the final node in the `instructions` list */
	*data_pointer = data; /* A pointer to the final node in the `data` list */
	FILE *ob; /* The `.ob` file being written to */


	while (fgets(line, MAX_LINE_LENGTH + NEWLINE_AND_NULL, am) != NULL) 
	{
		
		line_count++;
		line_too_long = handle_line_too_long(line, am, file_name, line_count, &errors_exist);
		
		if (line_too_long == True) 
   		continue;

		i = 0;
		is_label_def = 0;
		label_name = NULL;
		params = NULL;
		
		/* skip any white space at the beginning of the line */
		skip_white_space(line, &i);
		count = count_until(&line[i], ':');

		/* check if the current line contains a label definition */
		if (line[i + count] == ':') 
		{
    			count = handle_label_definition(line, &i, count, file_name, line_count, &label_name, &is_label_def, &errors_exist);
		}
 		else
		{	
			count = count_non_white_space(line, &i);
		}

		if (count == 0)
			continue;

		op_name = create_string_copy(&line[i-count], count);
		op_code = symbols_get(op_name);
		skip_white_space(line, &i);
		
		param_amount = symbols_get_param_amount(op_code);
		switch (parse_params(line, &i, param_amount, &params)) {
		case PARSER_EUNEXPECTEDSPACE:
			errors_exist = True;
			print_error(file_name, line_count, ERROR_PARAMSUNEXPECTEDSPACE);
			continue;
		case PARSER_EUNEXPECTEDCOMMA:
			errors_exist = True;
			print_error(file_name, line_count, ERROR_PARAMSUNEXPECTEDCOMMA);
			continue;
		case PARSER_ENOTENOUGHPARAMS:
			errors_exist = True;
			print_error(file_name, line_count, param_amount == PARAM_JUMP ? ERROR_PARAMSJUMP : ERROR_PARAMSNOTENOUGH, param_amount);
			continue;
		case PARSER_ETOOMANYPARAMS:
			errors_exist = True;
			print_error(file_name, line_count, param_amount == PARAM_JUMP ? ERROR_PARAMSJUMP : ERROR_PARAMSTOOMANY, param_amount);
			continue;
		case PARSER_EJUMPPARAMS:
			errors_exist = True;
			print_error(file_name, line_count, ERROR_PARAMSJUMP);
			continue;
		default:
			break;
		}
		/* if an error occurred, `params` will be NULL */


		/* check for duplicate label definitions (checks for `.entry` and `.extern` are done later) */
		if (is_label_def && (is_operation(op_name) || is_data_directive(op_code)))
		{
			label_attribute = hash_table_get_int(label_attributes, label_name);
			/* if the current label is already defined in the symbol table or is already marked as external, then it is a duplicate */
			if (hash_table_get_int(labels, label_name) != NULL || (label_attribute != NULL && *label_attribute & LABEL_EXTERNAL)) 
			{
				errors_exist = True;
				print_error(file_name, line_count, ERROR_LABELDEFINED, label_name);
			}
		}
	
		/* Handle directives, including data and string instructions, entry and extern directives */
		if (is_directive(op_name))
		{
			
			if (is_data_directive(op_code)) { /* `.string`, `.data` */
				if (is_label_def) {
					/* store this as a data label */
					hash_table_set_int(labels, label_name, data_count);
					hash_table_add_bit_to_field(label_attributes, label_name, LABEL_DATA);
				}

				if (op_code == DIRECTIVE_DATA) { /* `.data` */
					while (params != NULL) {
						if (!encode_num((char *) params->value, &data_pointer, &data_count)) {
							errors_exist = True;
							print_error(file_name, line_count, ERROR_DATAINVALIDNUMBER, (char *) params->value);
						}
						params = linked_list_free_next(params);
					}
				} else { /* `.string` */
					switch (encode_string(&line[i], &data_pointer, &data_count)) {
					case PARSER_EEXPECTEDQUOTES:
						errors_exist = True;
						print_error(file_name, line_count, ERROR_STRINGSTARTQUOTES);
						break;
					case PARSER_EUNFINISHEDSTRING:
						errors_exist = True;
						print_error(file_name, line_count, ERROR_STRINGUNFINISHED);
						break;
					case PARSER_EINVALIDCHAR:
						errors_exist = True;
						print_error(file_name, line_count, ERROR_STRINGASCII);
						break;
					default:
						break;
					}
				}
			} else { /* `.entry`, `.extern` */
				if (is_label_def) {
					print_warning(file_name, line_count, WARNING_LABELUSELESS, label_name);
					free(label_name);
					is_label_def = 0;
				}

				label_name = str_dupl((char *) params->value);
				label_attribute = hash_table_get_int(label_attributes, label_name);
				if (!is_valid_label(label_name)) {
					errors_exist = True;
					print_error(file_name, line_count, ERROR_LABELINVALIDNAME, label_name);
				/* check for duplicate entries and externals. this is not a duplicate if:
					1. this is an entry definition or the label isn't defined (we can't define an already defined label as an external, but we can define them as entries)
					2. label attributes either aren't stored for this label (they are stored only when defined), and if they are then the label isn't an entry and isn't an external */
				} else if ((op_code == DIRECTIVE_ENTRY || hash_table_get_int(labels, label_name) == NULL) && (label_attribute == NULL || !(*label_attribute & (LABEL_ENTRY | LABEL_EXTERNAL))))
					/* store this as an entry or external, depending on the op_code */
					hash_table_add_bit_to_field(label_attributes, label_name, (op_code == DIRECTIVE_EXTERN ? LABEL_EXTERNAL : LABEL_ENTRY));
				else {
					errors_exist = True;
					print_error(file_name, line_count, ERROR_LABELDEFINED, label_name);
				}
				free(label_name);
			}

		}
		else if (is_operation(op_name))
		{
			if (is_label_def) {
				/* store this as an instruction label */
				hash_table_set_int(labels, label_name, MEMORY_START + instruction_count);
				hash_table_add_bit_to_field(label_attributes, label_name, LABEL_INSTRUCTION);
			}

			switch (encode_operation(op_name, op_code, line_count, &params, &instruction_pointer, &instruction_count)) {
			case PARSER_EINVALIDNUMBER:
				print_error(file_name, line_count, ERROR_DATAINVALIDNUMBER, (char *) params->value);
				break;
			case PARSER_EINVALIDLABEL:
				print_error(file_name, line_count, ERROR_LABELINVALIDNAME, (char *) params->value);
				break;
			case PARSER_EINVALIDSOURCEPARAM:
				print_error(file_name, line_count, ERROR_OPINVALIDSOURCE, op_name);
				break;
			case PARSER_EINVALIDDESTPARAM:
				print_error(file_name, line_count, ERROR_OPINVALIDDEST, op_name);
				break;
			default:
				break;
			}
		} 	
		else 
		{
			/* here the given instruction isn't defined */
			errors_exist = True;
			print_error(file_name, line_count, ERROR_UNKNOWNINSTRUCTION, op_name);
		}

		if (is_label_def)
			free(label_name);
		free(op_name);
		linked_list_free(params);
	}

	/* add the instruction counter to all data labels */
	update_data_labels_with_instruction_count(labels, label_attributes, instruction_count);

	/* ensure that the program isn't using too much memory */
	if (instruction_count + data_count > MEMORY_END-MEMORY_START) {
		errors_exist = True;
		print_error(file_name, line_count, ERROR_BINARYOVERFLOW);
	}

	strcat(file_name, OB_SUFFIX);
	ob = open(file_name, "w");
	replace_extension(file_name, EMPTY_STRING);

	fprintf(ob, "%d %d\n", instruction_count, data_count);
	if (errors_exist==True) {
		close(ob);
		ob = NULL;
	}
	return ob;
}

void update_data_labels_with_instruction_count(struct hash_table *labels, struct hash_table *label_attributes, int instruction_count)
{
    	int i;
    	int *label_attribute;
	struct hash_node *attributes_node_pointer; /* pointer to a node in the `label_attributes` hash_table */

    	for (i = 0; i < HASH_TABLE_CAPACITY; i++)
	{
        	for (attributes_node_pointer = label_attributes->tab[i]; attributes_node_pointer != NULL; attributes_node_pointer = attributes_node_pointer->next)
		{
            		label_attribute = (int *) attributes_node_pointer->value;
            		if (label_attribute != NULL && *label_attribute & LABEL_DATA)
			{
                		hash_table_set_int(labels, attributes_node_pointer->key, *hash_table_get_int(labels, attributes_node_pointer->key) + MEMORY_START + instruction_count);
            		}
        	}
    	}
}

Bool handle_line_too_long(char *line, FILE *am, char *file_name, int line_count, Bool *errors_exist) 
{
    
	Bool line_too_long = False;

    	/* if we didn't encounter a newline and there are too much characters then the line is too long */
   	 while (strchr(line, '\n') == NULL && strlen(line) > MAX_LINE_LENGTH) 
	 {
        	if (line_too_long == False) 
		{
            		/* print the message */
            		print_error(file_name, line_count, ERROR_LINEOVERFLOW);
            		line_too_long = True;
            		*errors_exist = True;
        	}
        /* wait until the line ends */
        	if (fgets(line, MAX_LINE_LENGTH + NEWLINE_AND_NULL, am) == NULL) 
		{
            		break;
        	}
   	}

    	return line_too_long;
}

int handle_label_definition(char *line, int *i, int count, char *file_name, int line_count, char **label_name, int *is_label_def, Bool *errors_exist)
{
    *label_name = create_string_copy(&line[*i], count);

    /* label names can't be the same as a built-in constant */
    if (symbols_get(*label_name) != UNKNOWN_SYMBOL)
    {
        *errors_exist = True;
        print_error(file_name, line_count, ERROR_LABELSYMBOL, *label_name);
    }
    else if (!is_valid_label(*label_name))
    {
        *errors_exist = True;
        print_error(file_name, line_count, ERROR_LABELINVALIDNAME, *label_name);
    }
    else
        *is_label_def = 1;

    *i += count + 1;
    skip_white_space(line, i);
    return count_non_white_space(line, i);
}
