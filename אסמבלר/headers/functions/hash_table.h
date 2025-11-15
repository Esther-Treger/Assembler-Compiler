/* ------Functions that implement a hash table data structure.
The hash table can store integer and string values, and there are functions to get and set these values for a given key. Additionally, there is a function to add a bit to a bitfield value associated with a key in the hash table. The hash table can be dynamically resized as new elements are added or removed. Finally, there are functions to create a new hash table and to free the memory associated with a hash table----------------.
*/


/*Description: 
Creates a new hash table and initializes it with a size of 0 and an array of null pointers.
Returns: A pointer to the new hash table.*/
struct hash_table *hash_table_new(void);

/* Description: 
Frees the memory allocated for a hash table and all its nodes.
Parameters:
struct hash_table *m: A pointer to the hash table to free.
Returns:
None.*/
void hash_table_free(struct hash_table *);

/* Description: 
Retrieves the value of an integer stored in a hash table with the specified key.
Parameters:
struct hash_table *m: A pointer to the hash table to search in.
char *key: A pointer to the key to search for.
Returns: 
A pointer to the integer value with the specified key, or NULL if it doesn't exist.*/
int *hash_table_get_int(struct hash_table *, char *);

/*Description: 
Retrieves the value of a string stored in a hash table with the specified key.
Parameters:
struct hash_table *m: A pointer to the hash table to search in.
char *key: A pointer to the key to search for.
Returns: 
A pointer to the string value with the specified key, or NULL if it doesn't exist.*/
char *hash_table_get_str(struct hash_table *, char *);

/*Description: 
Sets the value of an integer in a hash table with the specified key.
Parameters:
struct hash_table *m: A pointer to the hash table to add the value to.
char *key: A pointer to the key to add the value under.
int value: The value to set.
Returns: 
A pointer to the value that was set.*/
void *hash_table_set_int(struct hash_table *, char *, int);

/* Description: 
Sets the value of a string in a hash table with the specified key.
Parameters:
struct hash_table *m: A pointer to the hash table to add the value to.
char *key: A pointer to the key to add the value under.
char *value: The string value to set.
Returns: 
A pointer to the value that was se*/
void *hash_table_set_str(struct hash_table *, char *k, char *v);

/* Description: 
Adds a bit to an integer stored in a hash table with the specified key.
Parameters:
struct hash_table *m: A pointer to the hash table to add the bit to.
char *key: A pointer to the key to add the bit to.
int value: The bit to add.
Returns: 
None.*/
void hash_table_add_bit_to_field(struct hash_table *, char *, int);


