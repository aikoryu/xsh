# TODO

## 2026-03-29 (in-code TODO reconciliation)

1. `High` `src/builtins.c:37`
   Implement `export` builtin behavior (argument parsing, environment update semantics, and error handling) instead of returning success without applying any change.

2. `Medium` `src/shell.c:19`
   Replace the experimental prompt placeholder with status-line rendering and command dispatch for experimental helpers (for example `last()`), so experimental mode is behaviorally distinct and testable.
