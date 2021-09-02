#include "point.h"
#include "execution_pointer.h"
typedef enum CommandType {
    KILL,
    CALL,
    WRITE
} CommandType;

typedef struct Command {
    CommandType command_type;
    // used for CALL command
    Point loc;
    // used for WRITE command
    Cell data;
} Command;

void run_command(ExecutionPointer* exec_ptr, Command* command, Grid* grid);