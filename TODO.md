# TODO

## 2026-03-28 (source + test coverage review)

1. `Medium` `tests/test_parse.c`
   Add parser tests for whitespace-only input and mixed whitespace tokenization (`" \t\n"` and `"echo\thello"`), since delimiter handling changed and those cases are currently unverified.

2. `Medium` `src/shell.c`
   Add shell-loop integration tests for the exec path (`execvp(parsed.args_arr[0], parsed.args_arr)`) and builtin/empty-input continue paths to ensure `_free_cmd_args` is always hit.

3. `Low` `src/parse.c:13`
   Reduce noisy startup output from `experimental_flag_set` so normal startup (`argc == 1`) does not print `Invalid flag, starting default shell`.
