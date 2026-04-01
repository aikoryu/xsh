#define _GNU_SOURCE
#include <linux/limits.h>
#include <unistd.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *construct_history_path() {
  char *home_dir = getpwuid(getuid())->pw_dir;
  char *xsh_history = ".xsh_history";
  char *buffer = NULL;
  int len = asprintf(&buffer, "%s/%s", home_dir, xsh_history);
  if (len < 0) {
    perror("failed to check for history");
    free(buffer);
    buffer = NULL;
    return NULL;
  }
  return buffer;
}

bool check_history_exists() {
  FILE *f;
  char* history = construct_history_path();
  if (history == NULL) {
      return false;
  }

  if ((f = fopen(history, "r")) != NULL) {
    fclose(f);
    return true;
  }

  return false;
}

/* We return true if history doesn't exist but is created here, we return false if history already exists and we open the file in append mode*/
bool create_history(bool history_exists) {
    if (history_exists) {
        return true;
    } else {
        FILE* f;
        char* history_file = construct_history_path();

        if ((f = fopen(history_file, "w")) == NULL) {
            fprintf(stderr, "Error: Could not create history file");
            return false;
        }

        fclose(f);
    }

    return true;
}

bool append_to_history(char* command) {
    FILE* f;
    char* history_file = construct_history_path();

    if ((f = fopen(history_file, "a")) == NULL) {
        fprintf(stderr, "Error: Could not open file to append to");
        return false;
    }

    fprintf(f, "%s", command);

    fclose(f);

    return true;
}
