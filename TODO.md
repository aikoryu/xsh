# TODO

## 2026-03-31 (in-code TODO reconciliation)

1. `Medium` `src/shell.c:13`
   Wire history writes and lookup into the shell loop so `last()` can execute an actual previous command from `.xsh_history` instead of placeholder behavior.
2. `Medium` `src/shell.c:21`
   Replace the experimental prompt placeholder with explicit experimental command routing (including `last()` behavior gating) and status-line rendering so experimental mode is behaviorally distinct and testable.
