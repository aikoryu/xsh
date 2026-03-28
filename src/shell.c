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

void shell_loop(bool experimental) {
    char* line = NULL;
    size_t n = 0;

    (void) experimental;

    while (true) {
        ssize_t bytes = getline(&line, &n, stdin);
        if (bytes == -1)
            break;

        struct CmdArgs parsed = _parse_command_line(line);

        if (parsed.count == 0)
            continue;

        if (_call_builtins(parsed))
            continue;

        pid_t child = fork();

        if (child < 0) {
            perror("failed to spawn child");
            exit(1);
        } else if (child == 0) {
            // TODO: this is going to require writing a tokenizer... so we should actually tokenize the input prior to forking
            execvp(line, &line);
        }

        waitpid(child, NULL, 0);
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
