#ifndef CODEGEN_H_
#define CODEGEN_H_
/** 
 * Allows for simple metaprogramming for C files, (basically code generation).
 * Replaces comments of the form "//@<i>" with 'expansions[i]' in the file with name file_name,
 * overriting the file with name out_file_name with the full expansion. As a simple example,
 * let's say we have a file 'file_template' with the following text:
 * hello
 * //@0
 * //@1
 * //@0
 *
 * Now let's say we call expand_file("file_template", "file.c", ["world\n", "foo bar baz\n"]);
 * 
 * file.c will look like:
 * hello
 * world
 * foo bar baz
 * world
 * 
 */
Result expand_file(char* file_name, char* out_file_name, char* expansions[], int n_expansions);
#endif