#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void test_parse_command_line_whitespace_only_input(void) {
    char input[] = " \t\n";
    CmdArgs parsed = _parse_command_line(input);

    assert(parsed.count == 0);
    assert(parsed.args_arr == NULL);
}

static void test_parse_command_line_non_empty_input(void) {
    char input[] = "echo hello";
    CmdArgs parsed = _parse_command_line(input);

    assert(parsed.count == 2);
    assert(parsed.args_arr != NULL);
    assert(strcmp(parsed.args_arr[0], "echo") == 0);
    assert(strcmp(parsed.args_arr[1], "hello") == 0);
    assert(parsed.args_arr[2] == NULL);

    _free_cmd_args(&parsed);
    assert(parsed.count == 0);
    assert(parsed.args_arr == NULL);
}

static void test_parse_command_line_mixed_whitespace_tokenization(void) {
    char input[] = "echo\thello";
    CmdArgs parsed = _parse_command_line(input);

    assert(parsed.count == 2);
    assert(parsed.args_arr != NULL);
    assert(strcmp(parsed.args_arr[0], "echo") == 0);
    assert(strcmp(parsed.args_arr[1], "hello") == 0);
    assert(parsed.args_arr[2] == NULL);

    _free_cmd_args(&parsed);
    assert(parsed.count == 0);
    assert(parsed.args_arr == NULL);
}

static void test_parse_command_line_strips_trailing_newline(void) {
    char input[] = "exit\n";
    CmdArgs parsed = _parse_command_line(input);

    assert(parsed.count == 1);
    assert(parsed.args_arr != NULL);
    assert(strcmp(parsed.args_arr[0], "exit") == 0);
    assert(parsed.args_arr[1] == NULL);

    _free_cmd_args(&parsed);
    assert(parsed.count == 0);
    assert(parsed.args_arr == NULL);
}

int main(void) {
    test_experimental_flag_set_accepts_enable_flag();
    test_experimental_flag_set_rejects_unknown_flag();
    test_experimental_flag_set_handles_negative_argc();
    test_parse_command_line_empty_input();
    test_parse_command_line_whitespace_only_input();
    test_parse_command_line_non_empty_input();
    test_parse_command_line_mixed_whitespace_tokenization();
    test_parse_command_line_strips_trailing_newline();

    puts("test_parse: all tests passed");
    return 0;
}
