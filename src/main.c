#include <stdbool.h>
#include "shell.h"
#include "parse.h"

int main(int argc, char* argv[]){
    bool experimental = experimental_flag_set(argc, argv);
    start_shell(experimental);

    return 0;
}
