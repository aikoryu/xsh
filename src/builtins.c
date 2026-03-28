#include <stdlib.h>
#include <string.h>
#include "parse.h"

bool _call_builtins(struct CmdArgs parsed) {
    if (parsed.count == 0 || parsed.args_arr == NULL || parsed.args_arr[0] == NULL)
        return false;

    if (strcmp(parsed.args_arr[0], "cd") == 0) {
        // chdir()
        return true;
    }

    if (strcmp(parsed.args_arr[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(parsed.args_arr[0], "export") == 0) {
        //export
        return true;
    }

    return false;
}
