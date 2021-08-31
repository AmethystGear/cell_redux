#ifndef CELL_REDUX_UTILS_H_
#define CELL_REDUX_UTILS_H_

#include <stdbool.h>
#include <stdio.h>
#include "error_handling/errors.h"

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define IGNORE_UNUSED(var) (void)(var)

char* int_to_string(int i);
char* append_string(char* str, char* to_append);
Result read_text_stream_to_string(FILE* file, char** str);
bool str_starts_with(char *str, char* pre);
bool str_ends_with(char *str, char* suf);
bool kth_bit_set(int n, int k);
char* strfmt(char *fmt, ...);
#endif
