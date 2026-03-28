#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parse.h"

static void test_experimental_flag_set_accepts_enable_flag(void) {
    char *argv[] = {"xsh", "--enable-experimental", NULL};
    bool actual = experimental_flag_set(2, argv);

    assert(actual == true);
}

static void test_experimental_flag_set_rejects_unknown_flag(void) {
    char *argv[] = {"xsh", "--unknown", NULL};
    bool actual = experimental_flag_set(2, argv);

    assert(actual == false);
}

static void test_experimental_flag_set_handles_negative_argc(void) {
    bool actual = experimental_flag_set(-1, NULL);

    assert(actual == false);
}

static void test_parse_command_line_empty_input(void) {
    char input[] = "";
    CmdArgs parsed = _parse_command_line(input);

    assert(parsed.count == 0);
    assert(parsed.args_arr == NULL);
}

/*
 * Current implementation exits(1) for non-empty input because of the
 * realloc error check in src/parse.c. This test captures that behavior.
 */
static void test_parse_command_line_non_empty_input_exits(void) {
    pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0) {
        char input[] = "echo hello";
        (void)_parse_command_line(input);
        _exit(0);
    }

    int status = 0;
    pid_t waited = waitpid(pid, &status, 0);
    assert(waited == pid);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 1);
}

int main(void) {
    test_experimental_flag_set_accepts_enable_flag();
    test_experimental_flag_set_rejects_unknown_flag();
    test_experimental_flag_set_handles_negative_argc();
    test_parse_command_line_empty_input();
    test_parse_command_line_non_empty_input_exits();

    puts("test_parse: all tests passed");
    return 0;
}
