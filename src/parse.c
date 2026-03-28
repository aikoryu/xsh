#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct CmdArgs {
    size_t count;
    char** args_arr;
};
bool experimental_flag_set(int argc, char* argv[]) {
    if (argc < 0) {
        return false;
    } else if (argc > 1 && strcmp(argv[1], "--enable-experimental") == 0)
        return true;

    printf("Invalid flag, starting default shell");
    return false;
}

struct CmdArgs _parse_command_line(char* s_args) {
    char** args_arr = NULL;
    const char* s_delim = " ";
    char* tok;
    int count = 0;

    tok = strtok(s_args, s_delim);

    while (tok != NULL) {
        char** temp = realloc(args_arr, (count + 2) * sizeof(char*));
        if (args_arr == NULL) {
            perror("realloc failed");
            exit(1);
        }

        args_arr = temp;

        args_arr[count] = malloc(strlen(tok) + 1);
        if (args_arr[count] == NULL) {
            perror("malloc failed");
            exit(1);
        }

        strcpy(args_arr[count], tok);
        count++;

        tok = strtok(NULL, s_delim);
    }

    if (args_arr != NULL)
        args_arr[count] = NULL;

    return (struct CmdArgs){
        .count = count,
        .args_arr = args_arr
    };

}
