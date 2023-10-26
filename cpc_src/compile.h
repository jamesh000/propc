#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

#define INPUT_NAME "vargs"

void compile(struct sexp *s, unsigned var_count, const char* output_name);
void compile_core(FILE *output_file, struct sexp *s);

#endif
