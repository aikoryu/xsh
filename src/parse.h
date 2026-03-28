#include <stddef.h>
#include <stdbool.h>

typedef struct CmdArgs {
    size_t count;
    char** args_arr;
} CmdArgs;
bool experimental_flag_set(int argc, char* argv[]);
CmdArgs _parse_command_line(char* s_args);
