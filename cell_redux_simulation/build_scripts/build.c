#include <stdlib.h>
#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>
#include <dirent.h>
#include "../src/utility/utils.h"
#include "../src/utility/vector/cvector.h"
#include "../src/utility/error_handling/errors.h"
#include "../src/utility/error_handling/signalhandler.h"
#include "../src/utility/codegen/codegen.h"

#define FIGHTER_DIR "../build/fighters/"

#define FIGHTER_INTERFACE_TEMPLATE "../build/fighter_interface/fighter_interface_c.templ"
#define FIGHTER_INTERFACE "../build/fighter_interface/fighter_interface.c"
        
/**
 * Given a file with C code in it, returns a string that contains that code, formatted by GNU indent.
 */
static char* get_formatted_code(char* code_file_name) {
    char* formatted_code;
    char* command = strfmt("astyle < %s", code_file_name);

    FILE* formatted_code_fp = popen(command, "r");
    free(command);

    fail_if_err(read_text_stream_to_string(formatted_code_fp, &formatted_code));
    fclose(formatted_code_fp);
    return formatted_code;
}

/**
 * Given GNU indent formatted C code, return the names of all the void functions.
 */
static Result get_void_function_names(char* formatted_code, cvector_vector_type(char*)* out) {
    cvector_vector_type(char*) void_functions = NULL;
    bool was_void = false;

    char * curr_line = formatted_code;
    while(curr_line) {
        // get next line
        char* next_line = strchr(curr_line, '\n');
        // temporarily terminate the current line
        if (next_line) *next_line = '\0';

        if (str_starts_with(curr_line, "static void")) {

            char* function_name_end;
            if ((function_name_end = strchr(curr_line, '(')) == NULL) {
                return result_err(ERR(strdup(curr_line)));
            }
            *function_name_end = '\0';
            cvector_push_back(void_functions, strdup(curr_line + strlen("static void ")));
            *function_name_end = '(';
        }

        // restore newline char  
        if (next_line) *next_line = '\n';
        // set current line to next line
        curr_line = next_line ? (next_line + 1) : NULL;
    }

    *out = void_functions;
    return result_ok();
}

static cvector_vector_type(char*) get_filenames_in_dir(char* dirname) {
    cvector_vector_type(char*) filenames = NULL;

    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                cvector_push_back(filenames, strfmt("%s/%s", dirname, dir->d_name));
            }
        }
        closedir(d);
    }
    return filenames;
}

static int compare_strings (const void* a, const void* b) {
    return strcmp(*((char**)a), *((char**)b));
}

/**
 * This script is responsible for creating the build directory
 */
int main(int argc, char* argv[]) {
    set_signal_handler();
    char* folder = argv[1];
    cvector_vector_type(char*) fighters = get_filenames_in_dir(folder);

    int num_fighters = cvector_size(fighters);
    char* fighter_names[num_fighters];
    cvector_vector_type(char*) all_void_functions = NULL;
    for (int i = 0; i < num_fighters; i++) {
        char* program_file = fighters[i];
        if (!str_ends_with(program_file, ".c")) {
            fail_on(ERR(strfmt("provided program is not a C program: %s", program_file)));
        }

        // run the file through GNU indent for easy parsing
        char* fighter_code = get_formatted_code(program_file);

        char* program_file_name = strdup(basename(program_file));
        char* program_file_header = strdup(program_file_name);
        program_file_header[strlen(program_file_header) - 1] = 'h';
        
        char* fighter_name = strdup(program_file_name);
        fighter_name[strlen(fighter_name) - 2] = '\0';
        fighter_names[i] = fighter_name;

        for (int j = 0; j < i; j++) {
            if (strcmp(fighter_names[j], fighter_name) == 0) {
                fail_on(ERR(strfmt("provided program has the same name as another program: %s", program_file)));
            }
        }

        cvector_vector_type(char*) void_functions;
        fail_if_err(get_void_function_names(fighter_code, &void_functions));
        for (size_t j = 0; j < cvector_size(void_functions); j++) {
            if (strcmp(void_functions[j], "init") == 0) {
                goto found_init;
            }
        }
        fail_on(ERR(strfmt("provided program does not have a 'void init()' function: %s", program_file)));
found_init:;

        char* fighter_header_code = strfmt(
            "#ifndef %s_\n#define %s_\n%s\n",
            fighter_name,
            fighter_name,
            "#include \"../world/stats.h\""
        );

        fighter_code = append_string(fighter_code, "\n");
        for (size_t j = 0; j < cvector_size(void_functions); j++) {
            char* function = strfmt("void %s_%s()", fighter_name, void_functions[j]);
            fighter_header_code = append_string(fighter_header_code, function);
            fighter_header_code = append_string(fighter_header_code, ";\n");
            int blah = 5;
            char* fighter_code_include = strfmt(
                "%s\n%s\n",
                "#include \"../world/world_interface.h\"",
                "#include \"../world/stats.h\""
            );
            fighter_code_include = append_string(fighter_code_include, fighter_code);
            free(fighter_code);
            fighter_code = fighter_code_include;

            fighter_code = append_string(fighter_code, function);
            fighter_code = append_string(fighter_code, "{\n");
            char* real_function_call = strfmt("    %s();\n", void_functions[j]);
            fighter_code = append_string(fighter_code, real_function_call);
            fighter_code = append_string(fighter_code, "}\n");

            free(real_function_call);
            free(function);
        }

        char* stats_extern_header = strfmt("extern Stats %s_stats;\n", fighter_name);
        fighter_header_code = append_string(fighter_header_code, stats_extern_header);
        fighter_header_code = append_string(fighter_header_code, "#endif\n");
        free(stats_extern_header);

        char* fighter_dir = strdup(FIGHTER_DIR);
        char* program_file_name_dir = append_string(fighter_dir, program_file_name);
        FILE* fighter_file = fopen(program_file_name_dir, "ab+");
        fputs(fighter_code, fighter_file);
        fclose(fighter_file);
        free(program_file_name_dir);

        fighter_dir = strdup(FIGHTER_DIR);
        char* program_file_header_name_dir = append_string(fighter_dir, program_file_header);
        FILE* fighter_header_file = fopen(program_file_header_name_dir, "ab+");
        fputs(fighter_header_code, fighter_header_file);
        fclose(fighter_header_file);
        free(program_file_header_name_dir);

        for (size_t j = 0; j < cvector_size(void_functions); j++) {
            cvector_push_back(all_void_functions, strfmt("%s_%s", fighter_name, void_functions[j]));
            free(void_functions[j]);
        }
        cvector_free(void_functions);
        free(program_file_name);
        free(program_file_header);
        free(fighter_code);
    }

    char* fighter_names_concat = strdup("");
    char* includes = strdup("");
    for (int i = 0; i < num_fighters; i++) {
        char* file_to_include = strfmt("#include \"../fighters/%s.h\"\n", fighter_names[i]);
        includes = append_string(includes, file_to_include);
        free(file_to_include);

        char* fighter_name = strfmt("\"%s\",\n", fighter_names[i]);
        fighter_names_concat = append_string(fighter_names_concat, fighter_name);
        free(fighter_name);
    }

    char* all_void_functions_sorted[cvector_size(all_void_functions)];
    for (int i = 0; i < ARRAY_LEN(all_void_functions_sorted); i++) {
        all_void_functions_sorted[i] = all_void_functions[i];
    }
    qsort(all_void_functions_sorted, ARRAY_LEN(all_void_functions_sorted), sizeof(char*), compare_strings);

    char* function_mappings = strdup("");
    for (int i = 0; i < ARRAY_LEN(all_void_functions_sorted); i++) {
        char* function_mapping = strfmt("{\"%s\", %s},\n", all_void_functions_sorted[i], all_void_functions_sorted[i]);
        function_mappings = append_string(function_mappings, function_mapping);
        free(function_mapping);
    }

    char* stat_mappings = strdup("");
    for (int i = 0; i < num_fighters; i++) {
        char* stat_mapping = strfmt("{\"%s\", &%s_stats},\n", fighter_names[i], fighter_names[i]);
        stat_mappings = append_string(stat_mappings, stat_mapping);
        free(stat_mapping);
    }

    char* fighter_interface_expansions[] = {
        includes,
        function_mappings,
        fighter_names_concat,
        stat_mappings
    };

    fail_if_err(expand_file(
        FIGHTER_INTERFACE_TEMPLATE,
        FIGHTER_INTERFACE,
        fighter_interface_expansions,
        ARRAY_LEN(fighter_interface_expansions)
    ));
}