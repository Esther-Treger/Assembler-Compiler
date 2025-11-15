/* -----functions that are used to manipulate a linked list data structure. ------*/


/* Description:
 
Creates a new node for a linked list with the given value and free function, and returns a pointer to the new node, 	it is assumed that the value can be `free()`'d 
Parameters:
void *value: A pointer to the value to be stored in the new node.
void (*mfree)(void *): A function pointer to the function to be used to free the value.
Returns: 
A pointer to the newly created node.*/
struct list_node *linked_list_new_node(void *, void (*)(void *));

/* Description: 
Frees the next node in a linked list and returns a pointer to the node that followed it.
Parameters:
struct list_node *n: A pointer to the node whose next node is to be freed.
Returns: 
A pointer to the node that followed the freed node, or NULL if the given node is NULL. */
struct list_node *linked_list_free_next(struct list_node *);

/*Description: 
Frees all nodes in a linked list, starting from the given node.
Parameters:
struct list_node *n: A pointer to the node at the start of the linked list.
Returns: None.
 */
void linked_list_free(struct list_node *);

