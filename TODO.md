# TODO

## 2026-03-31 (in-code TODO reconciliation)

1. `Medium` `src/shell.c:13`
   Wire history writes and lookup into the shell loop so `last()` can execute an actual previous command from `.xsh_history` instead of placeholder behavior.
2. `Medium` `src/shell.c:21`
   Replace the experimental prompt placeholder with explicit experimental command routing (including `last()` behavior gating) and status-line rendering so experimental mode is behaviorally distinct and testable.

## 2026-03-31 (history helper review follow-ups)

1. `High` `src/file.c:10`
   Guard `getpwuid(getuid())` before dereferencing `pw_dir` in `construct_history_path` and return a handled failure path when passwd lookup is unavailable so startup does not crash in minimal/container environments.
2. `High` `src/file.c:30`
   Replace the `fopen(..., "r")` existence probe in `check_history_exists` with a permission-independent check (for example `stat` or `access(..., F_OK)`) and ensure history creation avoids truncating an existing unreadable `.xsh_history` file.
