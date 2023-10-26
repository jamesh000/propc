#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    unsigned *var_count, (*P)(unsigned*);
    
    if (argc < 2) {
	fprintf(stderr, "Specify a library to open.\n");
	exit(1);
    }
    
    printf("Opening %s... ", argv[1]);
    fflush(stdout);
    void *P_lib = dlopen(argv[1], RTLD_NOW);
    if (P_lib == NULL) {
        fprintf(stderr, "failed: %s\n", dlerror());
        exit(1);
    }
    printf("opened.\n");
    
    printf("Fetching var_count and P()... ");
    var_count = dlsym(P_lib, "var_count");
    P = dlsym(P_lib, "P");
    if ((var_count == NULL) || (P == NULL)) {
	fprintf(stderr, "failed\n");
	exit(1);
    }
    printf("done. var_count=%d\n", *var_count);

    if ((unsigned)argc - 2 != *var_count) {
	fprintf(stderr, "Wrong number of arguments: %d, needed: %d\n", argc-2, *var_count);
	exit(1);
    }
    
    unsigned *arglist = malloc(*var_count * sizeof(unsigned));

    for (size_t i = 0; i < *var_count; i++) {
	const char *current_arg = argv[i+2];
	if (current_arg[1] != '\0') {
	    fprintf(stderr, "Invalid argument %s\n", argv[i+2]);
	    exit(1);
	} else {
	    switch (current_arg[0]) {
	    case 'T':
	    case 't':
		arglist[i] = 1u;
		break;
	    case 'F':
	    case 'f':
		arglist[i] = 0u;
		break;
	    default:
		fprintf(stderr, "Invalid argument %s\n", current_arg);
		exit(1);
	    }
	}
    }

    if ((*P)(arglist))
	printf("True.\n");
    else
	printf("False.\n");

    return 0;
}
