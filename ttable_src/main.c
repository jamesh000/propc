#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <math.h>

void print_block(unsigned vars, unsigned *input, unsigned result)
{
    for (unsigned bit_index = 0; bit_index < sizeof(unsigned); bit_index++) {
        printf("|");
        for (unsigned i = 0; i < vars; i++)
            printf(" %s |", (input[i] & (1u << bit_index)) ? "T" : "F");
        printf(" ----> %s\n", (result & (1u << bit_index)) ? "T" : "F");
    }
}


int main(int argc, char *argv[])
{
    unsigned *var_count, (*P)(unsigned*);
    unsigned rows, *input;
    size_t table_index;
    
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

    rows = 1 << *var_count;     /* Number of rows needed double with each variable */

    for (size_t i = 0; i < rows; i += sizeof(unsigned)) {
        input = calloc(sizeof(unsigned*), *var_count);

        // Create a block of unsigned integers with each bit representing a truth value
        for(size_t j = 0; j < *var_count; j++) {
            for (unsigned k = 0; k < sizeof(unsigned); k++) {
                table_index = i + k;
                if (table_index > rows) break;
                if ((table_index >> (*var_count-j-1)) & 1) // The larger the var, the larger the cycle between true and false
                    input[j] |= 1u << k;
            }
        }
        print_block(*var_count, input, (*P)(input));
        free(input);
    }
        
    return 0;
}
