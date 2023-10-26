#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <math.h>

int main(int argc, char *argv[])
{
    unsigned *var_count, (*P)(unsigned*);
    unsigned rows, **table, *results;
    
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

    rows = 1 << *var_count;

    for (unsigned i = 0; i < rows; i + sizeof(unsigned)) {
	
    }
    
    return 0;
}
