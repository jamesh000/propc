#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "bintree.h"
#include "compile.h"

#define VAR_MAP_START 2

int main(int argc, char *argv[])
{
    FILE *input_file;
    char c;

    struct sexp *root;
    struct var_registry registry;

    registry.current_map = VAR_MAP_START; // the values 0 and 1 are reserved for true and false
    registry.root = NULL;

    if (argc != 2) {
	fprintf(stderr, "Give the name of a single file to be compiled as an argument or '-' for stdin.\n");
	exit(1);
    }

    if (argv[1][0] == '-' && argv[1][1] == '\0')
	input_file = stdin;
    else if ((input_file = fopen(argv[1], "r")) == NULL) {
	fprintf(stderr, "Yer file is fucked.\n");
	exit(1);
    }

    while ((c = getc(input_file)) != '(') {
	if (c == EOF) {
	    fprintf(stderr, "The program that never begins?");
	    exit(1);
	}
    }

    root = parse(input_file, &registry);

    print_sexp(root);
    putchar('\n');

    if (valid_sexp(root)) {
	printf("Valid, compiling...\n");
    } else {
	fprintf(stderr, "Invalid, compilation is CANCELLED.\n");
	exit(1);
    }
    
    compile(root, registry.current_map-VAR_MAP_START, argv[1]);
    printf("Done compiling \"%s\", output file is \"%s%s\"\n", argv[1], argv[1], "_P.so");

    fclose(input_file);

    return 0;
}
