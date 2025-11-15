#include "data.h"

struct list_node *linked_list_new_node(void *value, void (*mfree)(void *))
{
	struct list_node *new_node = (struct list_node *) alloc(sizeof(*new_node));
	new_node->value = value;
	new_node->next = NULL;
	new_node->free = mfree;
	return new_node;
}

struct list_node *linked_list_free_next(struct list_node *node)
{
	struct list_node *tmp;
	if (node == NULL)
		return NULL;
	tmp = node->next;
	node->free(node->value);
	free(node);
	return tmp;
}

void linked_list_free(struct list_node *node)
{
	while (node != NULL)
		node = linked_list_free_next(node);
}
