#define _GNU_SOURCE
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *construct_history_path() {
  struct passwd *_passwd = getpwuid(getuid());
  const char *home_dir = NULL;

  if (_passwd != NULL && _passwd->pw_dir != NULL) {
    home_dir = _passwd->pw_dir;
  } else {
    home_dir = getenv("HOME");
  }

  if (home_dir == NULL || home_dir[0] == '\0') {
    fprintf(stderr, "Error: Could not get home directory\n");
    return NULL;
  }

  char *buffer = NULL;
  int len = asprintf(&buffer, "%s/.xsh_history", home_dir);
  if (len < 0 || buffer == NULL) {
    perror("failed to construct history path");
    free(buffer);
    return NULL;
  }

  return buffer;
}

bool check_history_exists() {
  char *history = construct_history_path();
  if (history == NULL) {
    fprintf(stderr, "Error: Could not determine history path\n");
    return false;
  }

  bool exists = access(history, F_OK) == 0;
  free(history);

  return exists;
}

/* Return true when history already exists or is successfully created.
 * Return false when history path construction or file creation fails. */
bool create_history(bool history_exists) {
  if (history_exists) {
    return true;
  } else {
    FILE *f;
    char *history_file = construct_history_path();
    if (history_file == NULL) {
      fprintf(stderr, "Error: Could not determine history path\n");
      return false;
    }

    if ((f = fopen(history_file, "w")) == NULL) {
      free(history_file);
      fprintf(stderr, "Error: Could not create history file\n");
      return false;
    }

    free(history_file);
    fclose(f);
  }

  return true;
}

bool append_to_history(char *command) {
  FILE *f;
  char *history_file = construct_history_path();
  if (history_file == NULL) {
    fprintf(stderr, "Error: Could not determine history path\n");
    return false;
  }

  if ((f = fopen(history_file, "a")) == NULL) {
    free(history_file);
    fprintf(stderr, "Error: Could not open file to append to\n");
    return false;
  }

  fprintf(f, "%s", command);

  free(history_file);
  fclose(f);

  return true;
}
