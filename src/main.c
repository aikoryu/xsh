#include <stdbool.h>
#include "shell.h"
#include "parse.h"
#include "file.h"

int main(int argc, char* argv[]){
    create_history(check_history_exists());
    bool experimental = experimental_flag_set(argc, argv);

    start_shell(experimental);

    return 0;
}
