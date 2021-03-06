/**
 * NOTE: this code is NOT portable to different operating systems, it only works on Linux.
 */
#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <err.h>
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
#include "backtrace.h"
#include "../utils.h"
#define STRINGIFY_LITERAL( x ) # x
#define STRINGIFY_EXPANDED( x ) STRINGIFY_LITERAL( x )
#define MAX_STACK_FRAMES 64

int addr2line(char const * const program_name, void const * const addr)
{
  char addr2line_cmd[512] = {0};

  /* have addr2line map the address to the relent line in the code */
  #ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr); 
  #else
    sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr); 
  #endif

  return system(addr2line_cmd);
}

  static void *stack_traces[MAX_STACK_FRAMES];
  void posix_print_stack_trace()
  {
    printf("posix\n");
    int i, trace_size = 0;
    char **messages = (char **)NULL;

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    messages = backtrace_symbols(stack_traces, trace_size);

    char* executable_name = strdup(STRINGIFY_EXPANDED(EXECUTABLE_NAME));

    for (i = 3; i < (trace_size - 1); ++i)
    {
      if (addr2line(executable_name, stack_traces[i]) != 0)
      {
        printf("  error determining line # for: %s\n", messages[i]);
      }

    }
    if (messages) { free(messages); } 
  }

void posix_signal_handler(int sig, siginfo_t *siginfo, void *context) {
    IGNORE_UNUSED(context);
    
    switch (sig) {
        case SIGSEGV:
            fputs("Caught SIGSEGV: Segmentation Fault\n", stderr);
            break;
            
        case SIGINT:
            fputs("Caught SIGINT: Interactive attention signal, (usually ctrl+c)\n", stderr);
            break;

        case SIGFPE:
            switch (siginfo->si_code) {
                case FPE_INTDIV:
                    fputs("Caught SIGFPE: (integer divide by zero)\n", stderr);
                    break;

                case FPE_INTOVF:
                    fputs("Caught SIGFPE: (integer overflow)\n", stderr);
                    break;

                case FPE_FLTDIV:
                    fputs("Caught SIGFPE: (floating-point divide by zero)\n", stderr);
                    break;

                case FPE_FLTOVF:
                    fputs("Caught SIGFPE: (floating-point overflow)\n", stderr);
                    break;

                case FPE_FLTUND:
                    fputs("Caught SIGFPE: (floating-point underflow)\n", stderr);
                    break;

                case FPE_FLTRES:
                    fputs("Caught SIGFPE: (floating-point inexact result)\n", stderr);
                    break;

                case FPE_FLTINV:
                    fputs("Caught SIGFPE: (floating-point invalid operation)\n", stderr);
                    break;

                case FPE_FLTSUB:
                    fputs("Caught SIGFPE: (subscript out of range)\n", stderr);
                    break;

                default:
                    fputs("Caught SIGFPE: Arithmetic Exception\n", stderr);
                    break;
            }
        case SIGILL:
            switch (siginfo->si_code) {
                case ILL_ILLOPC:
                    fputs("Caught SIGILL: (illegal opcode)\n", stderr);
                    break;

                case ILL_ILLOPN:
                    fputs("Caught SIGILL: (illegal operand)\n", stderr);
                    break;

                case ILL_ILLADR:
                    fputs("Caught SIGILL: (illegal addressing mode)\n", stderr);
                    break;

                case ILL_ILLTRP:
                    fputs("Caught SIGILL: (illegal trap)\n", stderr);
                    break;

                case ILL_PRVOPC:
                    fputs("Caught SIGILL: (privileged opcode)\n", stderr);
                    break;

                case ILL_PRVREG:
                    fputs("Caught SIGILL: (privileged register)\n", stderr);
                    break;

                case ILL_COPROC:
                    fputs("Caught SIGILL: (coprocessor error)\n", stderr);
                    break;

                case ILL_BADSTK:
                    fputs("Caught SIGILL: (internal stack error)\n", stderr);
                    break;

                default:
                    fputs("Caught SIGILL: Illegal Instruction\n", stderr);
                    break;
            }
            break;

        case SIGTERM:
            fputs("Caught SIGTERM: a termination request was sent to the program\n", stderr);
            break;

        case SIGABRT:
            fputs("Caught SIGABRT: usually caused by an abort() or assert()\n", stderr);
            break;

        default:
            break;
    }
    
posix_print_stack_trace();
    exit(1);
}

void set_signal_handler() {
    // set up alternate stack
    stack_t ss;
    ss.ss_sp = malloc(sizeof(uint8_t) * SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;

    if (sigaltstack(&ss, NULL) != 0) {
        err(1, "sigaltstack");
    }

    int signals[] = {SIGSEGV, SIGINT, SIGFPE, SIGILL, SIGTERM, SIGABRT};
    // register signal handlers
    struct sigaction sig_action = {};
    sig_action.sa_sigaction = posix_signal_handler;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    
    for (int i = 0; i < ARRAY_LEN(signals); i++) {
        if (sigaction(signals[i], &sig_action, NULL) != 0) {
            err(1, "sigaction");
        }
    }
}