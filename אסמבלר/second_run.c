#include "data.h"

Bool second_run(FILE *am, FILE *ob, char *file_name, struct list_node *instructions, struct list_node *data, struct hash_table *labels, struct hash_table *label_attributes)
{
	struct list_node *list_pointer = instructions->next; /* A pointer that refers to both the instructions and data linked lists. */
   	char *label_name, /* The name of the current label. */
        line[MAX_LINE_LENGTH + NEWLINE_AND_NULL], /* The current line in the code. */
        *current_operation; /* The operation in the current line. */
    	int *current_label_attribute, /* A value from `label_attributes` for the current label. */
        *current_label_value, /* A value from `labels` for the current label. */
        current_memory_address = MEMORY_START, /* The current memory address (14 bits) being printed. */
        line_count = 0, /* The current line number in the source code. */
        i, /* The current character index in the current line. */
        count; /* Results from `skip_white_space` and `count_non_white_space`. */
    	Bool errors_exist = False, /* Whether errors have been found. */
        is_data = False, /* A boolean indicating whether `list_pointer` is pointing to `data` or not. */
        ext_exist = False, /* A boolean indicating whether the code uses externals. */
        ent_exist = False; /* A boolean indicating whether the code defines entries. */
    	FILE *ent, *ext; /* Pointers to the .ent and .ext files. */
    	instruction *inst; /* The instruction pointed to by `list_pointer`, if it points to `instructions`. */

    	/* Open the .ent and .ext files. */
    	strcat(file_name, ENT_SUFFIX);
	ent = open(file_name, "w");
	replace_extension(file_name, EXT_SUFFIX);
	ext = open(file_name, "w");
	replace_extension(file_name, EMPTY_STRING);

    	/* Ensure valid `.entry` statements. */
    	while (fgets(line, MAX_LINE_LENGTH + NEWLINE_AND_NULL, am)) 
	{
	        line_count++;
	        i = 0;

        	/* Skip anything not regarding `.entry`. */
        	if ((current_operation = strstr(line, ".entry")) == NULL)
            		continue;

        	/* Skip from `.entry` to the label name. */
        	count_non_white_space(current_operation, &i);
        	skip_white_space(current_operation, &i);

        	/* Copy the label name and its attributes. */
        	count = count_non_white_space(current_operation, &i);
        	label_name = create_string_copy(&current_operation[i - count], count);
        	current_label_attribute = hash_table_get_int(label_attributes, label_name);

        	/* Ignore this if the label isn't defined as an entry. */
        	if (current_label_attribute != NULL && *current_label_attribute & LABEL_ENTRY) 
		{
            		/* This is valid if the label is defined as data or an instruction. */
            		if (*current_label_attribute & (LABEL_DATA | LABEL_INSTRUCTION)) 
			{
                		ent_exist = True;
                		fprintf(ent, "%s\t%d\n", label_name, *hash_table_get_int(labels, label_name));
            		} 
			else
			{
                		errors_exist = True;
                		print_error(file_name, line_count, ERROR_LABELNOTDEFINED, label_name);
            		}
        	}

        	free(label_name);
    	}

	/* ensure valid label usages and also output binary into the `.ob` file */
	line_count = 0;
	while (list_pointer != NULL) 
	{
		/* Output the binary count. */
		fprintf(ob, "0%d\t", current_memory_address);

		/* If `list_pointer` is pointing to an instruction. */
		if (is_data == False) 
		{
			label_name = NULL;
			inst = list_pointer->value;

			/* Save the `line_count` if the current word has one associated with it. */
			if (inst->line)
				line_count = inst->line;

			/* If this is a label then check its validity. */
			if (inst->is_label) 
			{
				/* Copy over the label name, value, and attributes. */
				label_name = (char *)inst->value;
				current_label_attribute = hash_table_get_int(label_attributes, label_name);
				current_label_value = hash_table_get_int(labels, label_name);

				/* Check if the label is invalid. It is invalid if it doesn't have a value (externals don't have one) and isn't an external. */
				if (current_label_value == NULL && (current_label_attribute == NULL || !(*current_label_attribute & LABEL_EXTERNAL))) 
				{
					errors_exist = True;
					print_error(file_name, line_count, ERROR_LABELNOTDEFINED, label_name);
				}

				/* Ensure that `label_attribute` isn't NULL for some reason. */
				if (current_label_attribute != NULL) 
				{
					/* If this is an external, simply output `............./`. */
					if (*current_label_attribute & LABEL_EXTERNAL) 	
					{
						encode(ENC_EXTERNAL, ob);
					ext_exist = True;
					fprintf(ext, "%s\t%d\n", label_name, current_memory_address);
					}
					else /* Otherwise, encode the label. */
					encode(encode_label(*current_label_value), ob);
				}
			}
			else /* A regular word to be printed. */
				encode(((word *)inst->value)->field, ob);

			/* If there are no more instructions, loop over `data`. */
			if (!list_pointer->next)
			{
				is_data = True;
				list_pointer = data;
			}
		}
		else /* Otherwise, this is data. */
			encode(((word *)list_pointer->value)->field, ob);

		fputc('\n', ob);

		current_memory_address++;
		list_pointer = list_pointer->next;

	}
	
	/* Close the .ob, .ent, and .ext files. */
	close(ob);
	close(ent);
	close(ext);
	
	/* If .ent does not exist, remove it. */
	if (ent_exist==False)
	{
		strcat(file_name, ENT_SUFFIX);
		remove(file_name);
		replace_extension(file_name, EMPTY_STRING);
	}
	
	/* If .ext does not exist, remove it. */
	if (ext_exist==False)
	{
		strcat(file_name, EXT_SUFFIX);
		remove(file_name);
		replace_extension(file_name, EMPTY_STRING);
	}
	
	/* Return whether errors exist. */
	return errors_exist;
}

void encode(unsigned int num, FILE *ob)
{
	int i;
	for (i = 1 << (WORD_LENGTH-1); i > 0; i /= 2)
		fputc((num & i) ? '/' : '.', ob);
}


