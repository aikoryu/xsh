#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "parse.h"

bool _call_builtins(struct CmdArgs parsed) {
    if (parsed.count == 0 || parsed.args_arr == NULL || parsed.args_arr[0] == NULL)
        return false;

    if (strcmp(parsed.args_arr[0], "cd") == 0) {
        if (parsed.count > 2) {
            perror("Too many arguments: USAGE: cd <path>");
            return true;
        } else if (parsed.count < 2) {
            char* home_dir = getpwuid(getuid()) -> pw_dir;
            int c = chdir(home_dir);
            if (c < 0) {
                perror("Not able to change directory");
            }
            return true;
        } else if (parsed.count == 2) {
            int c = chdir(parsed.args_arr[1]);
            if (c < 0) {
                perror("Not able to change directory");
            }
            return true;
        }
    }

    if (strcmp(parsed.args_arr[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(parsed.args_arr[0], "export") == 0) {
        if (parsed.count > 2) {
            fprintf(stderr, "Too many arguments: USAGE: export <VARNAME>=<VARDATA>\n");
            return true;
        } else if (parsed.count < 2) {
            fprintf(stderr, "Too few arguments: USAGE: export <VARNAME>=<VARDATA>\n");
            return true;
        } else if (parsed.count == 2) {
            ExportArgs s_export = _parse_export_args(parsed.args_arr[1]);
            if (s_export.count != 2 || s_export.name == NULL || s_export.value == NULL || s_export.name[0] == '\0') {
                fprintf(stderr, "Invalid argument: USAGE: export <VARNAME>=<VARDATA>\n");
                _free_export_args(&s_export);
                return true;
            }

            if (setenv(s_export.name, s_export.value, 1) != 0)
                perror("Not able to set environment variable");

            _free_export_args(&s_export);
            return true;
        }
        return true;
    }

    return false;
}
