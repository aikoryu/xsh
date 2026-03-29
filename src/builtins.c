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
        // TODO: implement export
        return true;
    }

    return false;
}
