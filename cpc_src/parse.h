#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include "bintree.h"

struct branch_sexp {
    char operator;
    unsigned operand_count;
    struct sexp **operands;
};

struct sexp {
    int is_leaf;
    union {
	unsigned leaf;
	struct branch_sexp subtree;
    } data;
};

struct sexp *parse(FILE *input_file, struct var_registry *registry);
void print_sexp(struct sexp* s);
int is_operator(char c);
int valid_sexp(struct sexp* s);


#endif
