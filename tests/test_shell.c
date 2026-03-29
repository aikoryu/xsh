#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "parse.h"
#include "shell.h"

typedef enum ParseScenario {
    PARSE_EMPTY,
    PARSE_BUILTIN,
    PARSE_EXTERNAL
} ParseScenario;

static ParseScenario g_parse_scenario;
static bool g_builtin_result;
static pid_t g_fork_result;
static int g_execvp_result;
static int g_lines_to_return;

static int g_parse_calls;
static int g_builtin_calls;
static int g_free_calls;
static int g_fork_calls;
static int g_waitpid_calls;
static int g_execvp_calls;
static int g_perror_calls;
static int g_getline_calls;
static int g_exit_called;
static int g_exit_status;
static pid_t g_waitpid_last_pid;
static const char *g_execvp_file;
static char *const *g_execvp_argv;

static jmp_buf g_exit_jmp;

static char *g_builtin_args[] = {"cd", NULL};
static char *g_external_args[] = {"echo", "hello", NULL};

static void reset_mocks(void) {
    g_parse_scenario = PARSE_EMPTY;
    g_builtin_result = false;
    g_fork_result = 1;
    g_execvp_result = -1;
    g_lines_to_return = 0;

    g_parse_calls = 0;
    g_builtin_calls = 0;
    g_free_calls = 0;
    g_fork_calls = 0;
    g_waitpid_calls = 0;
    g_execvp_calls = 0;
    g_perror_calls = 0;
    g_getline_calls = 0;
    g_exit_called = 0;
    g_exit_status = 0;
    g_waitpid_last_pid = -1;
    g_execvp_file = NULL;
    g_execvp_argv = NULL;
}

static ssize_t mock_readline(char **lineptr, size_t *n, FILE *stream) {
    (void)stream;
    g_getline_calls++;

    if (g_getline_calls > g_lines_to_return)
        return -1;

    if (*lineptr == NULL || *n < 4) {
        free(*lineptr);
        *n = 4;
        *lineptr = malloc(*n);
        assert(*lineptr != NULL);
    }

    memcpy(*lineptr, "x\n", 3);
    return 2;
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    return mock_readline(lineptr, n, stream);
}

ssize_t __getdelim(char **lineptr, size_t *n, int delimiter, FILE *stream) {
    (void)delimiter;
    return mock_readline(lineptr, n, stream);
}

CmdArgs _parse_command_line(char *s_args) {
    (void)s_args;
    g_parse_calls++;

    if (g_parse_scenario == PARSE_EMPTY) {
        return (CmdArgs){
            .count = 0,
            .args_arr = NULL
        };
    }

    if (g_parse_scenario == PARSE_BUILTIN) {
        return (CmdArgs){
            .count = 1,
            .args_arr = g_builtin_args
        };
    }

    return (CmdArgs){
        .count = 2,
        .args_arr = g_external_args
    };
}

void _free_cmd_args(CmdArgs *parsed) {
    g_free_calls++;
    parsed->count = 0;
    parsed->args_arr = NULL;
}

bool _call_builtins(struct CmdArgs parsed) {
    (void)parsed;
    g_builtin_calls++;
    return g_builtin_result;
}

pid_t fork(void) {
    g_fork_calls++;
    return g_fork_result;
}

pid_t waitpid(pid_t pid, int *status, int options) {
    (void)options;
    g_waitpid_calls++;
    g_waitpid_last_pid = pid;
    if (status != NULL)
        *status = 0;
    return pid;
}

int execvp(const char *file, char *const argv[]) {
    g_execvp_calls++;
    g_execvp_file = file;
    g_execvp_argv = argv;
    return g_execvp_result;
}

void perror(const char *s) {
    (void)s;
    g_perror_calls++;
}

void exit(int status) {
    g_exit_called = 1;
    g_exit_status = status;
    longjmp(g_exit_jmp, 1);
}

static void test_shell_loop_empty_input_frees_and_continues(void) {
    reset_mocks();
    g_parse_scenario = PARSE_EMPTY;
    g_lines_to_return = 1;

    shell_loop(false);

    assert(g_parse_calls == 1);
    assert(g_free_calls == 1);
    assert(g_builtin_calls == 0);
    assert(g_fork_calls == 0);
    assert(g_waitpid_calls == 0);
    assert(g_execvp_calls == 0);
}

static void test_shell_loop_builtin_path_frees_and_continues(void) {
    reset_mocks();
    g_parse_scenario = PARSE_BUILTIN;
    g_builtin_result = true;
    g_lines_to_return = 1;

    shell_loop(false);

    assert(g_parse_calls == 1);
    assert(g_builtin_calls == 1);
    assert(g_free_calls == 1);
    assert(g_fork_calls == 0);
    assert(g_waitpid_calls == 0);
    assert(g_execvp_calls == 0);
}

static void test_shell_loop_exec_parent_path_waits_and_frees(void) {
    reset_mocks();
    g_parse_scenario = PARSE_EXTERNAL;
    g_builtin_result = false;
    g_fork_result = 4242;
    g_lines_to_return = 1;

    shell_loop(false);

    assert(g_parse_calls == 1);
    assert(g_builtin_calls == 1);
    assert(g_fork_calls == 1);
    assert(g_waitpid_calls == 1);
    assert(g_waitpid_last_pid == 4242);
    assert(g_free_calls == 1);
    assert(g_execvp_calls == 0);
}

static void test_shell_loop_exec_child_path_calls_execvp_with_parsed_args(void) {
    reset_mocks();
    g_parse_scenario = PARSE_EXTERNAL;
    g_builtin_result = false;
    g_fork_result = 0;
    g_execvp_result = -1;
    g_lines_to_return = 1;

    if (setjmp(g_exit_jmp) == 0) {
        shell_loop(false);
        assert(false);
    }

    assert(g_parse_calls == 1);
    assert(g_builtin_calls == 1);
    assert(g_fork_calls == 1);
    assert(g_execvp_calls == 1);
    assert(g_execvp_file != NULL);
    assert(strcmp(g_execvp_file, "echo") == 0);
    assert(g_execvp_argv != NULL);
    assert(strcmp(g_execvp_argv[0], "echo") == 0);
    assert(strcmp(g_execvp_argv[1], "hello") == 0);
    assert(g_execvp_argv[2] == NULL);
    assert(g_perror_calls == 1);
    assert(g_exit_called == 1);
    assert(g_exit_status == 1);
    assert(g_waitpid_calls == 0);
    assert(g_free_calls == 0);
}

int main(void) {
    test_shell_loop_empty_input_frees_and_continues();
    test_shell_loop_builtin_path_frees_and_continues();
    test_shell_loop_exec_parent_path_waits_and_frees();
    test_shell_loop_exec_child_path_calls_execvp_with_parsed_args();

    puts("test_shell: all tests passed");
    return 0;
}
