#include "parse.h"
#include <assert.h>


void add_leaf(unsigned value, struct branch_sexp *b)
{
    struct sexp *leaf = (struct sexp*)malloc(sizeof(struct sexp));
    leaf->is_leaf = 1;
    leaf->data.leaf = value;
    
    b->operand_count++;
    b->operands = (struct sexp**)realloc(b->operands, b->operand_count * sizeof(struct sexp*));
    b->operands[b->operand_count-1] = leaf;
}


struct sexp *parse(FILE *input_file, struct var_registry *registry)
{
    struct sexp *s = (struct sexp*)malloc(sizeof(struct sexp));
    s->is_leaf = 0;

    struct branch_sexp *b = &(s->data.subtree);
    b->operator = getc(input_file);
    b->operand_count = 0;
    b->operands = (struct sexp**)malloc(1);

    // Things can be parsed if very strange ways if parens are eaten into the operator slot
    if (b->operator == '(') {
	fprintf(stderr, "If I allow '(' to be lexed as an operator normally it leads to wacky behavior. This is for your own good.\n");
	exit(1);
    }

    // Empty list '()' is equal to false
    if (b->operator == ')') {
	printf("Just warning you that because I am very funny the empty list is equal to False.\n");
	struct sexp *leaf = (struct sexp*)malloc(sizeof(struct sexp));
	leaf->is_leaf = 1;
	leaf->data.leaf = 0;
	return leaf;
    }

    char c;
    while ((c = getc(input_file)) != EOF) {
	if ('0' <= c && c <= '9') {
	    unsigned mapped_var_id = place_var(read_var_id(&c, input_file), registry);
	    if (c == EOF) break; // In case the last character read by read_var_id was EOF

            add_leaf(mapped_var_id, b);
	}
	// read_var_id places first non number character in c, so the next iteration can immediately begin
	    
	switch (c) {
	case ' ':
	case '\n':
	case '\t':
	    break;
	case '(':
	    b->operand_count++;
	    b->operands = (struct sexp**)realloc(b->operands, b->operand_count * sizeof(struct sexp*));
	    b->operands[b->operand_count-1] = parse(input_file, registry);
	    break;
	case ')':
	    return s;
	    break;
        case 'F':
            add_leaf(0, b);
            break;
        case 'T':
            add_leaf(1, b);
            break;
	default:
	    fprintf(stderr, "Invalid input character '%c'\n", c);
            exit(1);
	}
    }

    fprintf(stderr, "Your parenthesis are unbalanced LOL\n");
    exit(1);
}


// Prints out a sexp basically as the user wrote it
void print_sexp(struct sexp* s)
{
    if (s->is_leaf) {
        switch (s->data.leaf) {
        case 0:
            putchar('F');
            break;
        case 1:
            putchar('T');
            break;
        default:
            printf("%d", s->data.leaf - 2);
        }
    } else {
	putchar('(');
	putchar(s->data.subtree.operator); 
	for (unsigned i = 0; i < s->data.subtree.operand_count; i++) {
	    putchar(' ');
	    print_sexp(s->data.subtree.operands[i]);
	}
	putchar(')');
    }
}


// Checks if some character is a valid operator
int is_operator(char c)
{
    switch (c) {
    case '!':
    case '&':
    case '|':
    case '?':
    case '=':
    case '^':
	return 1;
    default:
	return 0;
    }
}


// Verifies that a given expression is valid
int valid_sexp(struct sexp* s)
{
    if (s->is_leaf) {
	if (is_operator(s->data.leaf)) {
	    fprintf(stderr, "Wayward operator %c detected\n", s->data.leaf);
	    return 0;
	}
	return 1;
    } else {
	struct branch_sexp *b = &(s->data.subtree);
	int status = 1;
	
	if (!is_operator(b->operator)) {
	    fprintf(stderr, "%c is not an operator.\n", b->operator);
	    status = 0;
	} else if (b->operand_count == 0) {
	    fprintf(stderr, "Every operator takes at least one operand. Culprit: %c\n", b->operator);
	    status = 0;
	} else if (b->operator != '!' && b->operand_count == 1) {
	    fprintf(stderr, "That operator (%c) takes more than one operand.\n", b->operator);
	    status = 0;
	} else if (b->operator == '!' && b->operand_count > 1) {
	    fprintf(stderr, "Negation (!) can only take one operand.\n");
	    status = 0;
	} else if (b->operator == '?' && b->operand_count > 2) {
	    fprintf(stderr, "Implication (?) cannot take more than 2 operands. Nest them yourself.\n");
	    status = 0;
	}
	
	for (unsigned i = 0; i < b->operand_count; i++) {
	    if (!valid_sexp(b->operands[i]))
		status = 0;
	}
	return status; 
    }
}
