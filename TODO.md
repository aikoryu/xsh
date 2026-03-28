# TODO

## Review Findings

1. `Critical` `src/parse.c:29-31`  
   `_parse_command_line` checks `args_arr == NULL` after `realloc`, which causes non-empty input to fail and `exit(1)` on the first token.

2. `High` `src/shell.c:39`  
   `execvp` is called as `execvp(line, &line)` instead of using tokenized argv (`parsed.args_arr`), so command execution is incorrect.

3. `Medium` `src/shell.c:24-30`  
   Parsed argument buffers are never freed in the shell loop (including `continue` paths), causing per-command memory leaks.

4. `Low` `src/parse.c:16`  
   `experimental_flag_set` prints `Invalid flag, starting default shell` even for normal startup with no flag (`argc == 1`), producing noisy output.
