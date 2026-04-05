#include <assert.h>
#include <pwd.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "file.h"

static bool g_passwd_available;
static const char *g_mock_home;
static struct passwd g_mock_passwd;

static int g_exit_called;
static int g_exit_status;
static jmp_buf g_exit_jmp;

static void reset_mocks(void) {
    g_passwd_available = false;
    g_mock_home = NULL;
    g_mock_passwd = (struct passwd){0};
    g_exit_called = 0;
    g_exit_status = 0;
}

uid_t getuid(void) {
    return 1000;
}

struct passwd *getpwuid(uid_t uid) {
    (void)uid;
    if (!g_passwd_available)
        return NULL;
    return &g_mock_passwd;
}

char *getenv(const char *name) {
    if (strcmp(name, "HOME") == 0)
        return (char *)g_mock_home;
    return NULL;
}

void exit(int status) {
    g_exit_called = 1;
    g_exit_status = status;
    longjmp(g_exit_jmp, 1);
}

static void test_construct_history_path_prefers_passwd_directory(void) {
    reset_mocks();
    g_passwd_available = true;
    g_mock_passwd.pw_dir = "/passwd-home";
    g_mock_home = "/env-home";

    char *path = construct_history_path();

    assert(path != NULL);
    assert(strcmp(path, "/passwd-home/.xsh_history") == 0);
    free(path);
}

static void test_construct_history_path_falls_back_to_home_env(void) {
    reset_mocks();
    g_passwd_available = false;
    g_mock_home = "/env-home";

    char *path = construct_history_path();

    assert(path != NULL);
    assert(strcmp(path, "/env-home/.xsh_history") == 0);
    free(path);
}

static void test_construct_history_path_returns_null_when_no_home_source(void) {
    reset_mocks();
    g_passwd_available = false;
    g_mock_home = NULL;

    if (setjmp(g_exit_jmp) == 0) {
        char *path = construct_history_path();
        assert(path == NULL);
        assert(g_exit_called == 0);
        return;
    }

    assert(false);
}

static void test_construct_history_path_returns_null_when_home_is_empty(void) {
    reset_mocks();
    g_passwd_available = false;
    g_mock_home = "";

    if (setjmp(g_exit_jmp) == 0) {
        char *path = construct_history_path();
        assert(path == NULL);
        assert(g_exit_called == 0);
        return;
    }

    assert(false);
}

static void test_create_history_failure_is_non_fatal_when_home_missing(void) {
    reset_mocks();
    g_passwd_available = false;
    g_mock_home = NULL;

    if (setjmp(g_exit_jmp) == 0) {
        bool created = create_history(false);
        assert(created == false);
        assert(g_exit_called == 0);
        return;
    }

    assert(false);
}

int main(void) {
    test_construct_history_path_prefers_passwd_directory();
    test_construct_history_path_falls_back_to_home_env();
    test_construct_history_path_returns_null_when_no_home_source();
    test_construct_history_path_returns_null_when_home_is_empty();
    test_create_history_failure_is_non_fatal_when_home_missing();

    puts("test_file: all tests passed");
    return 0;
}
