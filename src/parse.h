#ifndef XSH_SRC_PARSE_H
#define XSH_SRC_PARSE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct CmdArgs {
    size_t count;
    char** args_arr;
} CmdArgs;

typedef struct ExportArgs {
    size_t count;
    char* name;
    char* value;
} ExportArgs;

bool experimental_flag_set(int argc, char* argv[]);
CmdArgs _parse_command_line(char* s_args);
void _free_cmd_args(CmdArgs *parsed);
ExportArgs _parse_export_args(char* s_export);
void _free_export_args(ExportArgs *p_export);

#endif
