#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parse.h"
#include "builtins.h"

// TODO: implement .xsh_history so that the last() command has a last command to target

void shell_loop(bool experimental) {
    char* line = NULL;
    size_t n = 0;

    while (true) {
        if (experimental == true) {
            // TODO: This should include get_status_line and also allow experimental commands like last() to be called
            // TODO: scaffolding for last is built out, might ultimately have to split the actual processing of commands here as well for
            // for example, if last is called in experimental mode it should just run the last command `if (experimental == false)`
            // last shouldn't be available
            printf("\nxsh> ");
        } else {
            printf("\nxsh> ");
        }

        ssize_t bytes = getline(&line, &n, stdin);
        if (bytes == -1)
            break;

        struct CmdArgs parsed = _parse_command_line(line);

        if (parsed.count == 0) {
            _free_cmd_args(&parsed);
            continue;
        }

        if (_call_builtins(parsed)) {
            _free_cmd_args(&parsed);
            continue;
        }

        pid_t child = fork();

        if (child < 0) {
            perror("failed to spawn child");
            exit(1);
        } else if (child == 0) {
            execvp(parsed.args_arr[0], parsed.args_arr);
            perror("execvp failed");
            exit(1);
        }

        waitpid(child, NULL, 0);
        _free_cmd_args(&parsed);
    }

    free(line);
}

void start_experimental_shell() {
    shell_loop(true);
}

void start_shell(bool experimental) {
    if (experimental) {
        start_experimental_shell();
    } else {
        shell_loop(false);
    }
}
