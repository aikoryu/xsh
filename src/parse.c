#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parse.h"

bool experimental_flag_set(int argc, char* argv[]) {
    if (argc < 0) {
        return false;
    } else if (argc > 1 && strcmp(argv[1], "--enable-experimental") == 0)
        return true;

    return false;
}

CmdArgs _parse_command_line(char* s_args) {
    char** args_arr = NULL;
    const char* s_delim = " \t\r\n\v\f";
    char* tok;
    int count = 0;

    tok = strtok(s_args, s_delim);

    while (tok != NULL) {
        char** temp = realloc(args_arr, (count + 2) * sizeof(char*));
        if (temp == NULL) {
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

    return (CmdArgs){
        .count = count,
        .args_arr = args_arr
    };

}

void _free_cmd_args(CmdArgs *parsed) {
    for (size_t i = 0; i < parsed->count; i++) {
        free(parsed->args_arr[i]);
    }
    free(parsed->args_arr);

    parsed->count = 0;
    parsed->args_arr = NULL;
}

ExportArgs _parse_export_args(char* s_export) {
   ExportArgs parsed = {
       .count = 0,
       .name = NULL,
       .value = NULL
   };

   if (s_export == NULL) {
       return parsed;
   }

   char* eq = strchr(s_export, '=');
   if (eq == NULL) {
       parsed.name = malloc(strlen(s_export) + 1);
       if (parsed.name == NULL) {
           perror("malloc failed");
           return parsed;
       }

       strcpy(parsed.name, s_export);
       parsed.count = 1;
       return parsed;
   }

   size_t name_len = (size_t)(eq - s_export);
   size_t value_len = strlen(eq + 1);

   parsed.name = malloc(name_len + 1);
   parsed.value = malloc(value_len + 1);
   if (parsed.name == NULL || parsed.value == NULL) {
       perror("malloc failed");
       free(parsed.name);
       free(parsed.value);
       parsed.name = NULL;
       parsed.value = NULL;
       return parsed;
   }

   memcpy(parsed.name, s_export, name_len);
   parsed.name[name_len] = '\0';
   memcpy(parsed.value, eq + 1, value_len + 1);
   parsed.count = 2;

   return parsed;
}

void _free_export_args(ExportArgs *p_export) {
    free(p_export->name);
    free(p_export->value);

    p_export->count = 0;
    p_export->name = NULL;
    p_export->value=NULL;
}
