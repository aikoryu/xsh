#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parse.h"
#include "builtins.h"

static void test_call_builtins_rejects_empty_command(void) {
    CmdArgs parsed = {
        .count = 0,
        .args_arr = NULL,
    };

    assert(_call_builtins(parsed) == false);
}

static void test_call_builtins_rejects_null_first_argument(void) {
    char *args[] = {NULL};
    CmdArgs parsed = {
        .count = 1,
        .args_arr = args,
    };

    assert(_call_builtins(parsed) == false);
}

static void test_call_builtins_handles_cd(void) {
    char *args[] = {"cd", NULL};
    CmdArgs parsed = {
        .count = 1,
        .args_arr = args,
    };

    assert(_call_builtins(parsed) == true);
}

static void test_call_builtins_handles_export(void) {
    char *args[] = {"export", NULL};
    CmdArgs parsed = {
        .count = 1,
        .args_arr = args,
    };

    assert(_call_builtins(parsed) == true);
}

static void test_call_builtins_rejects_unknown_command(void) {
    char *args[] = {"pwd", NULL};
    CmdArgs parsed = {
        .count = 1,
        .args_arr = args,
    };

    assert(_call_builtins(parsed) == false);
}

static void test_call_builtins_exit_terminates_process(void) {
    pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0) {
        char *args[] = {"exit", NULL};
        CmdArgs parsed = {
            .count = 1,
            .args_arr = args,
        };
        (void)_call_builtins(parsed);
        _exit(42);
    }

    int status = 0;
    pid_t waited = waitpid(pid, &status, 0);
    assert(waited == pid);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
}

int main(void) {
    test_call_builtins_rejects_empty_command();
    test_call_builtins_rejects_null_first_argument();
    test_call_builtins_handles_cd();
    test_call_builtins_handles_export();
    test_call_builtins_rejects_unknown_command();
    test_call_builtins_exit_terminates_process();

    puts("test_builtins: all tests passed");
    return 0;
}
