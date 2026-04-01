#include <stdio.h>
#include <stdbool.h>

char* construct_history_path();
bool check_history_exists();
bool create_history(bool history_exists);
bool append_to_history(char* command);
