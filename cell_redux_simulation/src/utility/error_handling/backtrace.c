/**
 * NOTE: this code is NOT portable to different operating systems, it only works on Linux.
 */
#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <err.h>
#include <execinfo.h>
#include "../utils.h"
#include "errors.h"
#define STRINGIFY_LITERAL( x ) # x
#define STRINGIFY_EXPANDED( x ) STRINGIFY_LITERAL( x )
#define MAX_STACK_FRAMES 256

char* get_backtrace(int ignore_first, int ignore_last) {
    
    void *trace[MAX_STACK_FRAMES];
    int trace_size = backtrace(trace, MAX_STACK_FRAMES);
    char** messages = backtrace_symbols(trace, trace_size);

    char* output_lines = strdup("Backtrace:\n");
    #ifndef EXECUTABLE_NAME
        output_lines = append_string(output_lines, "warning: EXECUTABLE_NAME not specified during compilation, ");
        output_lines = append_string(output_lines, "so line numbers are not available.\n");
    #endif

    for (int i = 0; i < trace_size; i++) {
        // get backtrace with function name
        char* bt_message = strfmt("[bt] #%d %s\n", i, messages[i]);
        output_lines = append_string(output_lines, bt_message);
        free(bt_message);

        // get file name and line numbers (if EXECUTABLE_NAME is defined)
        #ifdef EXECUTABLE_NAME
            // We need enough space for '0x' + [pointer] + '\0', that's the size of a pointer + 3 bytes.
            char* ptr_str = strfmt("%p", trace[i]);
            char* executable_name = strdup(STRINGIFY_EXPANDED(EXECUTABLE_NAME));
            char* command = strfmt("addr2line %s -e %s", ptr_str, executable_name);
            FILE* addr2line_command = popen(command, "r");
            free(command);
            free(executable_name);
            free(ptr_str);

            char* addr2line_output;
            if (read_text_stream_to_string(addr2line_command, &addr2line_output).is_err) {
                fprintf(stderr, "could not get backtrace");
                exit(1);
            }

            output_lines = append_string(output_lines, addr2line_output);
            free(addr2line_output);
        #endif
    }
    return output_lines;
}