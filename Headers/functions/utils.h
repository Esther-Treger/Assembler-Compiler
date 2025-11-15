/* -----functions for managing and manipulating strings------*/


/*The create_string_copy function creates a copy of the input string s, up to a maximum length specified by lim.
The function returns a pointer to the newly allocated copy of the input string s, or NULL if the allocation fails.*/
char *str_dupl(char *);


/*The str_dupl function creates a copy of the input null-terminated string s.
 The function returns a pointer to the newly allocated copy of the input string s, or NULL if the allocation fails.*/
char *create_string_copy(char *, size_t);



/*The replace_extension function replaces the extension of the input string s with the new extension ext*/
void replace_extension(char *s, char *ext);


