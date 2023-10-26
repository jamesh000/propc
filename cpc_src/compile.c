#include "compile.h"



void compile(struct sexp *s, unsigned var_count, const char *output_name)
{
    const char *clang_command = "clang -x c -shared - -o ";
    char *command = malloc(strlen(clang_command) + strlen(output_name) + 5);
    strcpy(command, clang_command);
    strcat(command, output_name);
    strcat(command, "_P.so");
    
    FILE *output_file = popen(command, "w");
    free(command);
    
    fprintf(output_file, "const unsigned var_count = %d;\n", var_count); /* Add symbol for number of variables */
    fprintf(output_file, "unsigned P(unsigned *vargs) { return ");
    compile_core(output_file, s);
    fprintf(output_file, "; }\n");
    putchar(EOF);
}

void compile_core(FILE *output_file, struct sexp *s)
{
    if (s->is_leaf) {
        fflush(stdout);
        switch (s->data.leaf) {
        case 0:
            fputc('0', output_file);
            break;
        case 1:
            fputc('t', output_file);
            break;
        default:
            fprintf(output_file, "%s[%d]", INPUT_NAME, s->data.leaf-2);
        }
    } else {
        fflush(stdout);
        fputc('(', output_file);

        switch (s->data.subtree.operator) {
        case '!':
            fputc('~', output_file);
            compile_core(output_file, s->data.subtree.operands[0]);
            break;
        case '?':
            fputc('~', output_file);
            compile_core(output_file, s->data.subtree.operands[0]);
            fputc('^', output_file);
            compile_core(output_file, s->data.subtree.operands[1]);
            break;
        default:
        {
            unsigned i;
            for (i = 0; i < s->data.subtree.operand_count-1; i++) {
                compile_core(output_file, s->data.subtree.operands[i]);
                fputc(s->data.subtree.operator, output_file);
            }
            compile_core(output_file, s->data.subtree.operands[i]);
        }
        }
        fputc(')', output_file);
    }
}


