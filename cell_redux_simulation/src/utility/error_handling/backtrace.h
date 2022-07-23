#ifndef BACKTRACE_H_
#define BACKTRACE_H_
/**
 * Returns the backtrace leading up to this call.
 * This function can only provide line numbers if the executable name is provided at compile time,
 * otherwise it provides function names + addresses. The executable name should be specified at compile time
 * using a flag of the form "-DEXECUTABLE_NAME=<executable_name>".
 * 
 * ingnore_first: first N stack frames to ignore
 * ignore_last: last N stack frames to ignore
 */
char* get_backtrace(int ignore_first, int ignore_last);
#endif