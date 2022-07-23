#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "../utils.h"
#include "../error_handling/errors.h"

#define EXPANSION_PREFIX "@"

Result expand_file(char* file_name, char* out_file_name, char* expansions[], int n_expansions) {
    FILE* in_file = fopen(file_name, "r");
    if (in_file == NULL) {
        return result_err(ERR(READ_FILE_ERROR));
    }

    FILE* out_file = fopen(out_file_name, "w+");
    if (out_file == NULL) {
        return result_err(ERR(READ_FILE_ERROR));
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
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
            char* str = strdup(expansions[expansion_index]);
            fputs(str, out_file);
            free(str);
        } else {
            fputs(line, out_file);
        }
    }
    fclose(in_file);
    fclose(out_file);
    return result_ok();
}