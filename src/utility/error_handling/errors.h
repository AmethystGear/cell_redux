#ifndef CELL_REDUX_ERRORS_H_
#define CELL_REDUX_ERRORS_H_
#define ERR(X) _Generic((X), int: err_enum, char*: err_str, default: err_create)(X)

/**
 * Provides an Error/Result interface for potentially recoverable errors.
 */

#include <stdlib.h>
#include <stdbool.h>
/**
 * enum containing all application-level errors
 */
typedef enum ErrorType {
    TIMED_OUT,
    NULL_POINTER,
    BUFFER_NOT_BIG_ENOUGH,
    READ_PIPE_ERROR,
    COULD_NOT_PARSE_JSON,
    INCORRECT_JSON,
    READ_FILE_ERROR,
    FUNCTION_DOES_NOT_EXIST
} ErrorType;

/**
 * struct containing the line and file where the error occurred.
 */
typedef struct Error {
    char* backtrace;
    char* message;
    ErrorType error_type;
    int flag; // check to see if message/error_type is valid
} Error;

typedef struct ErrorToMessage {
    ErrorType error_type;
    char* error_message;
} ErrorToMessage;

typedef struct Result {
    bool is_err;
    Error error;
} Result;

void fail_on(Error error);
void fail_if_err(Result result);
void fail_if_null(void *ptr);

Error err_enum(ErrorType error);
Error err_str(char* message);
Error err_create(ErrorType error_type, char* message);

void print_error(Error error);
void err_delete(Error error);

Result result_err(Error error);
Result result_ok();
#endif