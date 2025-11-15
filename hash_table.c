#include "data.h"

/* Computes a hash value for a given string */
unsigned int hash(char *key)
{

	unsigned int hash_value;
	for (hash_value = 0; *key != '\0'; key++)
		hash_value = *key + 31 * hash_value;
	return hash_value % HASH_TABLE_CAPACITY;
}

/* Initializes a new hash table */
struct hash_table *hash_table_new(void)
{
	int i;
	struct hash_table *new_table = (struct hash_table *) alloc(sizeof(*new_table));

	/* Initialize the hash table size */
	new_table->size = 0;

	/* Initialize the `tab` array to avoid seg fault */
	for (i = 0; i < HASH_TABLE_CAPACITY; i++)
		new_table->tab[i] = NULL;

	return new_table;
}

/* Frees the memory used by a hash table */
void hash_table_free(struct hash_table *m)
{

	struct hash_node *node, *tmp;
	int i;

	if (m == NULL)
		return;

	/* Loop through each bucket in the hash table */
	for (i = 0; i < HASH_TABLE_CAPACITY; i++) {
		node = m->tab[i];

		/* Loop through each node in the current bucket */
		while (node != NULL) {
			tmp = node->next;
			free(node->key);
			free(node->value);
			free(node);
			node = tmp;
		}

		/* Set the bucket to NULL */
		m->tab[i] = NULL;
	}

	/* Free the hash table itself */
	free(m);
}

/* Returns a pointer to a hash node for a given key */
struct hash_node *get_node(struct hash_table *m, char *key)
{

	struct hash_node *n;

	if (m == NULL)
		return NULL;

	/* Loop through the nodes in the bucket corresponding to the given key */
	for (n = m->tab[hash(key)]; n != NULL; n = n->next)
		if (strcmp(n->key, key) == 0)
			return n;

	/* Return NULL if the key is not found */
	return NULL;
}

/* Returns an integer value associated with a given key */
int *hash_table_get_int(struct hash_table *m, char *key)
{
	struct hash_node *n;

	/* If the hash table or hash node is NULL, return NULL */
	if (m == NULL)
		return NULL;

	/* Otherwise, get the hash node associated with the key */
	n = get_node(m, key);

	/* Return NULL if the hash node is NULL */
	if (n == NULL)
		return NULL;

	/* Otherwise, return a pointer to the integer value associated with the hash node */
	return (int *) n->value;
}

/* Returns a string value associated with a given key */
char *hash_table_get_str(struct hash_table *m, char *key)
{
	struct hash_node *n;

	/* If the hash table or hash node is NULL, return NULL */
	if (m == NULL)
		return NULL;

	/* Otherwise, get the hash node associated with the key */
	n = get_node(m, key);

	/* Return NULL if the hash node is NULL */
	if (n == NULL)
		return NULL;

	/* Otherwise, return a pointer to the string value associated with the hash node */
	return (char *) n->value;
}

/* prepares a new node and returns it to later have its value set */
struct hash_node *prepare_node(struct hash_table *m, char *key)
{
	/* creates a new hash node with the specified key */
	struct hash_node *n;
	unsigned int hash_value;
	if (m == NULL)
		return NULL;
	/* if the key is not already in the hash table, create a new node for it */
	if ((n = get_node(m, key)) == NULL) {
		n = (struct hash_node *) alloc(sizeof(*n));
		n->key = (char *) alloc(sizeof(char) * strlen(key));
		strcpy(n->key, key);
		hash_value = hash(key);
		n->next = m->tab[hash_value];
		m->tab[hash_value] = n;
		m->size++;
	} else
		free(n->value);
	return n;
}

void *hash_table_set_int(struct hash_table *m, char *key, int value)
{
	/* sets the value associated with the given key to the specified integer value */
	struct hash_node *new_node;
	if (m == NULL || (new_node = prepare_node(m, key)) == NULL)
		return NULL;
	new_node->value = alloc(sizeof(value));
	*((int *) new_node->value) = value;
	return new_node->value;
}

void *hash_table_set_str(struct hash_table *m, char *key, char *value)
{
	/* sets the value associated with the given key to the specified string value */
	struct hash_node *new_node;
	if (m == NULL || (new_node = prepare_node(m, key)) == NULL)
		return NULL;
	new_node->value = alloc(sizeof(char) * strlen(value));
	strcpy(new_node->value, value);
	return new_node->value;
}

void hash_table_add_bit_to_field(struct hash_table *m, char *key, int value)
{
	/* adds a bitfield value to the existing value associated with the given key */
	int *existing_value;
	if (m == NULL)
		return;
	/* get the existing value associated with the given key */
	existing_value = hash_table_get_int(m, key);
	/* if no existing value is found, set the value to the given parameter, otherwise add the value to the existing one */
	hash_table_set_int(m, key, existing_value == NULL ? value : ((*existing_value) | value));
}
