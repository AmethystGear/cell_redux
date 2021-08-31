#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"
#include "error_handling/errors.h"

/**
 * Converts an int to a string.
 * The returned string must be freed by the caller.
 */
char* int_to_string(int i) {
    int len = snprintf(NULL, 0, "%d", i);
    char* str = malloc(sizeof(char*) * (len + 1));
    snprintf(str, len + 1, "%d", i);
    return str;
}

/**
 * Appends to_append to str, reallocating more memory for str if neccesary. Returns new pointer to str.
 */
char* append_string(char* str, char* to_append) {
    int str_len = strlen(str);
    int to_append_len = strlen(to_append);
    int total_size = str_len + to_append_len + 1;
    char* new_str = realloc(str, total_size);
    memcpy(new_str + str_len, to_append, to_append_len + 1);
    return new_str;
}

/**
 * Reads a text stream to a string.
 * Keeps the string in the str parameter, keeps the length of this string in the len parameter.
 * Returns OK if successful, otherwise READ_FILE_ERROR.
 */
Result read_text_stream_to_string(FILE* file, char** str) {
    if (file == NULL) {
        return result_err(ERR(READ_FILE_ERROR));
    }
    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim(&buffer, &len, '\0', file);
    if (bytes_read == -1) {
        return result_err(ERR(READ_FILE_ERROR));
    }
    *str = buffer;
    return result_ok();
}

/**
 * Returns true if str starts with pre
 */
bool str_starts_with(char *str, char *pre) {
    if (!str || !pre) return false;
    size_t str_len = strlen(str);
    size_t pre_len = strlen(pre);
    if (pre_len > str_len) return false;
    return strncmp(pre, str, strlen(pre)) == 0;
}

/**
 * Returns true if str sends with suf
 */
bool str_ends_with(char *str, char *suf) {
    if (!str || !suf) return false;
    size_t str_len = strlen(str);
    size_t suf_len = strlen(suf);
    if (suf_len > str_len) return false;
    return strncmp(str + str_len - suf_len, suf, suf_len) == 0;
}

/**
 * Returns true if the kth bit is set, where the first bit is the 0th bit.
 */
bool kth_bit_set(int n, int k) {
    return n & (1 << k);
}

/**
 * Returns a formatted string.
 */
char* strfmt(char *fmt, ...) {
    char* out;
    va_list args;
    va_start(args, fmt);
    if (vasprintf(&out, fmt, args) == -1) {
        return NULL;
    }
    return out;
}