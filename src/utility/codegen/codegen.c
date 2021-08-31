#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../utils.h"
#include "../error_handling/errors.h"
#define EXPANSION_PREFIX "@"

Result expand_file(char* file_name, char* out_file_name, char* expansions[], int n_expansions) {
    printf("template: %s\n", file_name);
    printf("template  : %s\n", out_file_name);
    FILE* in_file = fopen(file_name, "r");
    printf("sad\n");
    if (in_file != NULL) {
        printf("sad1\n");
    } else {
        printf("sad2\n");
        return result_ok();
    }

    FILE* out_file = fopen(out_file_name, "w+");

    Result err;
    char* line;
    size_t len;
    ssize_t read;
    while ((read = getline(&line, &len, in_file)) != -1) {
        if (str_starts_with(line, EXPANSION_PREFIX)) {
            char* ptr;
            errno = 0;
            long expansion_index_l = strtol(line + strlen(EXPANSION_PREFIX), &ptr, 10);
            if (*ptr != '\n' || expansion_index_l < 0 || expansion_index_l > n_expansions) {
                fclose(in_file);
                fclose(out_file);
                return result_err(ERR(strfmt("cannot expand index %d, valid indexes are from 0 to %d inclusive", n_expansions - 1)));
            }
            int expansion_index = expansion_index_l;
            fputs(expansions[expansion_index], out_file);
          
        } else {
            fputs(line, out_file);
        }
    }
    fclose(in_file);
    fclose(out_file);
    return result_ok();
}