# TODO

## 2026-03-31 (in-code TODO reconciliation)

1. `Medium` `src/shell.c:13`
   Wire history writes and lookup into the shell loop so `last()` can execute an actual previous command from `.xsh_history` instead of placeholder behavior.
2. `Medium` `src/shell.c:21`
   Replace the experimental prompt placeholder with explicit experimental command routing (including `last()` behavior gating) and status-line rendering so experimental mode is behaviorally distinct and testable.

## 2026-04-01 (history path fallback follow-up)

1. `Medium` `src/file.c:9`
   Add the `$HOME` environment-variable fallback in `construct_history_path` when `getpwuid(getuid())` returns `NULL` so history path construction can still succeed in minimal/container environments.
