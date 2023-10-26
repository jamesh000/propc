#include "bintree.h"


/*
  Reads an int from the input file.
  char c is used to pass the first character to read_var, and the last character back to parse.
*/
unsigned read_var_id(char *c, FILE *input_file)
{
    unsigned var_id = 0;
    do {
	var_id = 10*var_id + (unsigned)(*c - '0');
    } while ((*c = getc(input_file)) != EOF && '0' <= *c && *c <= '9');

    return var_id;
}

/*
  Takes a variable identifier and searches for it in the registry of variables.
  If the identifier is found, it just exits. Otherwise, it creates a new entry for that identifier.
  The registry is organized into a binary tree where the value of every node on its left side is less than
  its value, and the value of every node on its right side is greater than its value.
*/
unsigned place_var(unsigned var_id, struct var_registry *registry)
{
    struct node **current_node = &(registry->root);

    while (*current_node != NULL) {
        if (var_id == (*current_node)->value) // match found
            return (*current_node)->map;
        else if (var_id > (*current_node)->value) // walk right
            current_node = &((*current_node)->right);
        else if (var_id < (*current_node)->value) // walk left
            current_node = &((*current_node)->left);
    }
    
    *current_node = (struct node*)malloc(sizeof(struct node));
    
    (*current_node)->value = var_id;
    (*current_node)->map = registry->current_map;
    (*current_node)->left = NULL;
    (*current_node)->right = NULL;
    
//    printf("Adding %d, map is %d\n", (*current_node)->value, (*current_node)->map);

    registry->current_map++;

    return (*current_node)->map;
}
