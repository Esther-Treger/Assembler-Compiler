#include "data.h"

FILE *macro_layout(FILE *as, char *file_name)
{
	char line[MAX_LINE_LENGTH + NEWLINE_AND_NULL], /* current source code line including null terminator and possible newline */
	*new_line_pointer, /* used for removing final newline in macros (otherwise there will be duplicates) */
	*current_macro_name , /*  name name in current macro definition */
	*current_macro_content ; /* macro content in current macro definition or replacement */
	int i, /* current character in current line */
	count, /* results from `skip_white_space` and `count_non_white_space` */
	line_off_set, /* used for printing parts of the current line to avoid printing macro names */
	in_macro_definition  = 0; /* whether we are in a macro definition */
	struct hash_table *macros = hash_table_new();
	FILE *am;
	strcat(file_name, AM_SUFFIX);
	am = open(file_name, "w");

	while (fgets(line, MAX_LINE_LENGTH + NEWLINE_AND_NULL, as) != NULL)
	{
		line_off_set = 0;
		i = 0;

		skip_white_space(line, &i);
		count = count_non_white_space(line, &i);

		/* ignore comments and empty lines */
		if (line[i-count] == ';' || count == 0)
			continue;

		if (!in_macro_definition )
			if (is_valid_mcr(line, &i, &count, &current_macro_name )) 			{
				current_macro_content = macro_content_alloc(as);
				in_macro_definition  = 1;
			}
			else
			{
				/* while the line contains more content */
				while (count > 0)
				{
					current_macro_name = create_string_copy(&line[i-count], count);
					/* if a macro of this name exists and a built-in constant of the same name doesn't exist */
					if ((current_macro_content  = hash_table_get_str(macros, current_macro_name )) != NULL && symbols_get(current_macro_name ) == UNKNOWN_SYMBOL) {
						line[i-count] = '\0'; /* to print the current line up to the macro name */
						fputs(&line[line_off_set], am);
						fputs(current_macro_content , am); /* print the macro content */
						line_off_set = i; /* offset the line to print the current line after the macro name */
					}
					free(current_macro_name );
					if (skip_white_space(line, &i) == 0)
						break;
					count = count_non_white_space(line, &i);
				}

				fputs(&line[line_off_set], am);
			}
			else if (is_valid_end_mcr(line, &i, &count))
			{
				in_macro_definition  = 0;
				/* remove final newline to avoid duplicates */
				new_line_pointer = strrchr(current_macro_content , '\n');
				if (new_line_pointer != NULL)
					*new_line_pointer = '\0';
				hash_table_set_str(macros, current_macro_name , current_macro_content );
				/* free because the hash_table copied everything */
				free(current_macro_name );
				free(current_macro_content );
			}
			else
				strcat(current_macro_content , line);
	}

	hash_table_free(macros);

	close(am);
	am = open(file_name, "r");
	replace_extension(file_name, EMPTY_STRING);

	return am;
}

char *macro_content_alloc(FILE *as)
{
	char line[MAX_LINE_LENGTH + NEWLINE_AND_NULL]; /* current line in file */
	int i, /* current character in current line */
		lines = 0, /* amount of lines before nearest `endmcr` */
		count = 0; /* results from `skip_white_space` and `count_non_white_space` */
	fpos_t p; /* save current position to return to after counting lines */
	char *ptr; /* the memory will be allocated here */

	fgetpos(as, &p);

	/* count lines until we hit a valid `endmcr` */
	while (fgets(line, MAX_LINE_LENGTH + NEWLINE_AND_NULL, as) != NULL) {
		i = 0;
		skip_white_space(line, &i);
		count = count_non_white_space(line, &i);
		if (is_valid_end_mcr(line, &i, &count))
			break;
		lines++;
	}

	fsetpos(as, &p);

	ptr = (char *) alloc(sizeof(char) * ((MAX_LINE_LENGTH + 1) * lines) + 1);
	*ptr = '\0';

	return ptr;
}

int is_valid_mcr(char *line, int *i, int *count, char **current_macro_name )
{
	/* if the given string between the indices `i` and `count` are equal to and of the same length as `mcr` */
	if (*count != sizeof(MCR)-1 || strncmp(MCR, &line[(*i)-(*count)], *count) != 0)
		return 0;
	if (skip_white_space(line, i) == 0)
		return 0;
	/* get the macro name */
	if ((*count = count_non_white_space(line, i)) == 0)
		return 0;
	*current_macro_name = create_string_copy(&line[(*i)-(*count)], *count);
	skip_white_space(line, i);
	/* make sure there's nothing after the macro name */
	return count_non_white_space(line, i) == 0;
}

int is_valid_end_mcr(char *line, int *i, int *count)
{
	/* if the given string between the indices `i` and `count` are equal to and of the same length as `endmcr` */
	if (*count != sizeof(ENDMCR)-1 || strncmp(ENDMCR, &line[(*i)-(*count)], *count) != 0)
		return 0;
	skip_white_space(line, i);
	return count_non_white_space(line, i) == 0;
}
