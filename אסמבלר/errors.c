#include "data.h"

void *alloc(size_t s)
{
	void *ptr = malloc(s);
	if (ptr == NULL) {
		printf("error: failed to allocate memory\n");
		exit(1);
	}
	return ptr;
}

FILE *open(char *file_name, char *mode)
{
	FILE *f = fopen(file_name, mode);
	if (f == NULL) {
		printf("error: failed to open file (%s)\n", file_name);
		exit(1);
	}
	return f;
}

int close(FILE *file)
{
	int i = fclose(file);
	if (i == EOF) {
		printf("error: failed to close file\n");
		exit(1);
	}
	return i;
}

char **error_messages;

void error_util_prepare(void)
{
	error_messages = (char **) alloc(sizeof(*error_messages ) * ERRORCOUNT);

	error_messages [ERROR_LABELINVALIDNAME] = "label names can't be more than 30 characters long, and must start with a letter followed by letters or numbers (%s)";
	error_messages [ERROR_LABELSYMBOL] = "label name must not be a pre-defined symbol (%s)";
	error_messages [ERROR_LABELDEFINED] = "label is already defined (%s)";
	error_messages [ERROR_LABELNOTDEFINED] = "label is not defined (%s)";
	error_messages [WARNING_LABELEMPTY] = "label defined without instruction (%s)";
	error_messages [WARNING_LABELUSELESS] = "useless label definition (%s)";

	error_messages [ERROR_PARAMSUNEXPECTEDSPACE] = "unexpected space in params list";
	error_messages [ERROR_PARAMSUNEXPECTEDCOMMA] = "unexpected comma in params list";
	error_messages [ERROR_PARAMSNOTENOUGH] = "not enough parameters (expected %d)";
	error_messages [ERROR_PARAMSTOOMANY] = "too many parameters (expected %d)";
	error_messages [ERROR_PARAMSJUMP] = "jump operations must receive a label followed by either 2 parameters inside parentheses, or no parameters at all";

	error_messages [ERROR_OPINVALIDSOURCE] = "invalid source parameter for %s";
	error_messages [ERROR_OPINVALIDDEST] = "invalid destination parameter for %s";
	error_messages [ERROR_DATAINVALIDNUMBER] = "invalid number (%s)";

	error_messages [ERROR_STRINGSTARTQUOTES] = "string declarations must start with quotes (\")";
	error_messages [ERROR_STRINGUNFINISHED] = "unfinished string (strings must end with quotes \")";
	error_messages [ERROR_STRINGASCII] = "strings must only contain printable ascii characters";

	error_messages [ERROR_LINEOVERFLOW] = "line exceeds max length of 80";
	error_messages [ERROR_BINARYOVERFLOW] = "binary file exceeds max memory of 256";

	error_messages [ERROR_UNKNOWNINSTRUCTION] = "unknown instruction (%s)";
}

void error_util_free(void)
{
	free(error_messages );
}



void print_warning(char *file_name, int line, enum err_util_errno e, ...)
{
	va_list args;
	char *s = error_messages[e];
	va_start(args, e);
	final_print("Warning:", file_name, line, s, args);
	va_end(args);
}

void print_error(char *file_name, int line, enum err_util_errno e, ...)
{
	va_list args;
	char *s = error_messages [e];
	va_start(args, e);
	final_print("Error:", file_name, line, s, args);
	va_end(args);
}

void final_print(char *prefix, char *file_name, int line, char *s, va_list args)
{
	printf("%s: %s.am:%d - ", prefix, file_name, line);
	vprintf(s, args);
	printf("\n");
}
