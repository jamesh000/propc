/*
  I decided to keep track of my variables with a binary tree.
  This is partially because they are ints, so I can construct an ad hoc binary tree and do (maybe) O(logn) searches.
  Whether the searches are actually O(logn) depends on the variables that compound proposition uses and the order they occur in.
  Most likely, the user will just count up, making this approach effectively O(n).
  This bring us to the main reason I did things like this: I just wanted to use something more complex than an array.
*/

#ifndef BINTREE_H
#define BINTREE_H

#include <stdlib.h>
#include <stdio.h>

struct node {
    unsigned value;
    unsigned map;
    struct node *left;
    struct node *right;
};

struct var_registry {
    unsigned current_map;
    struct node *root;
};

unsigned read_var_id(char *c, FILE *input_file);
unsigned place_var(unsigned var, struct var_registry *registry);

#endif
