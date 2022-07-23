#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <err.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "errors.h"
#include "../utils.h"
#include "backtrace.h"
#include "signalhandler.h"

ErrorToMessage err_mapping[] = {
    {TIMED_OUT, "timed out while performing operation"},
    {NULL_POINTER, "pointer was unexpectedly null"},
    {BUFFER_NOT_BIG_ENOUGH, "buffer was not big enough for input"},
    {READ_PIPE_ERROR, "error while reading from pipe"},
    {COULD_NOT_PARSE_JSON, "the provided string was not in proper json format"},
    {INCORRECT_JSON, "expecting different json value than what was recieved"},
    {READ_FILE_ERROR, "error while reading from file"},
    {FUNCTION_DOES_NOT_EXIST, "cannot call a function with this name because no function exists with this name"}
};

void fail_on(Error error) {
    print_error(error);
    err_delete(error);
    abort();
}

void fail_if_err(Result result) {
    if(result.is_err) {
        fail_on(result.error);
    }
}

void fail_if_null(void* ptr) {
    if (ptr == NULL) {
        fail_on(err_enum(NULL_POINTER));
    }
}

char* get_error_msg(ErrorType error_type) {
    for (int i = 0; i < ARRAY_LEN(err_mapping); i++) {
        ErrorToMessage mapping = err_mapping[i];
        if (mapping.error_type == error_type) {
            return mapping.error_message;
        }
    }
    return NULL;
}


void print_error(Error error) {
    if (kth_bit_set(error.flag, 0)) {
        char* err_message = get_error_msg(error.error_type);
        if (err_message != NULL) {
            printf("%s\n", err_message);
        } else {
            printf("no error message for error type <%d>", error.error_type);
        }
    }
    if (kth_bit_set(error.flag, 1)) {
        printf("%s\n", error.message);
    }
    if (error.backtrace == NULL) {
        printf("no backtrace found\n");
    } else {
        printf("%s", error.backtrace);
    }
}

Error err_enum(ErrorType error_type) {
    Error error = {
        .message = NULL,
        .error_type = error_type, 
        .flag = 1, 
        .backtrace = get_backtrace(2, 3)
    };
    return error;
}

Error err_str(char* message) {
    Error error = {
        .message = message,
        .flag = 2, 
        .backtrace = get_backtrace(2, 3)
    };
    return error;
}

Error err_create(ErrorType error_type, char* message) {
    Error error = {
        .message = message, 
        .error_type = error_type,
        .flag = 3, 
        .backtrace = get_backtrace(2, 3)
    };
    return error;
}

void err_delete(Error error) {
    free(error.backtrace);
    free(error.message);
}


Result result_err(Error error) {
    Result res;
    res.error = error;
    res.is_err = true;
    return res;
}

Error err_null() {
    Error error = {
        .message = NULL,
        .flag = 0,
        .backtrace = NULL
    };
    return error;
}

Result result_ok() {
    Result res;
    res.error = err_null();
    res.is_err = false;
    return res;
}

void result_delete(Result result) {
    err_delete(result.error);
}