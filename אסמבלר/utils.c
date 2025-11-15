#include "data.h"

char *create_string_copy(char *s, size_t lim)
{
	char *n = (char *) alloc(sizeof(char) * (lim + 1));
	strncpy(n, s, lim);
	n[lim] = '\0';
	return n;
}

char *str_dupl(char *s)
{
	return create_string_copy(s, strlen(s));
}

void replace_extension(char *s, char *ext)
{
	*strrchr(s, '.') = '\0';
	strcat(s, ext);
}
