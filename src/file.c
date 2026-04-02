#define _GNU_SOURCE
#include <linux/limits.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO: Fall back to $HOME if getpwuid() fails
char *construct_history_path() {
  struct passwd *_passwd = getpwuid(getuid());
  char *home_dir;
  if (_passwd == NULL) {
    return NULL;
  } else {
    home_dir = _passwd->pw_dir;
  }
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
  const char *history = construct_history_path();
  if (history == NULL) {
    fprintf(stderr, "Error: Could not get passwd");
    return false;
  }

  int check_access;

  if ((check_access = access(history, F_OK) == 0)) {
    return true;
  }

  return false;
}

/* We return true if history doesn't exist but is created here, we return false
 * if history already exists and we open the file in append mode*/
bool create_history(bool history_exists) {
  if (history_exists) {
    return true;
  } else {
    FILE *f;
    char *history_file = construct_history_path();
    if (history_file == NULL) {
      fprintf(stderr, "Error: Could not get passwd");
      return false;
    }

    if ((f = fopen(history_file, "w")) == NULL) {
      fprintf(stderr, "Error: Could not create history file");
      return false;
    }

    fclose(f);
  }

  return true;
}

bool append_to_history(char *command) {
  FILE *f;
  char *history_file = construct_history_path();
  if (history_file == NULL) {
    fprintf(stderr, "Error: Could not get passwd");
    return false;
  }

  if ((f = fopen(history_file, "a")) == NULL) {
    fprintf(stderr, "Error: Could not open file to append to");
    return false;
  }

  fprintf(f, "%s", command);

  fclose(f);

  return true;
}
