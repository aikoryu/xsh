# Changelog

All notable changes to this project will be documented in this file.

- [2026-03-28] - Added header include guards - Added `#ifndef/#define/#endif` guards in `src/parse.h`, `src/shell.h`, and `src/builtins.h` to prevent duplicate-inclusion issues.
- [2026-03-28] - Fixed `execvp` argv wiring - In `shell_loop` (`src/shell.c`), changed child execution to `execvp(parsed.args_arr[0], parsed.args_arr)` so the process gets a valid program name and NULL-terminated argv.
- [2026-03-28] - Added child exec error path - In `src/shell.c`, added `perror("execvp failed")` and `exit(1)` after `execvp` to handle execution failures explicitly in the child.
- [2026-03-28] - Added parsed-argument cleanup in shell loop - `src/shell.c` now calls `_free_cmd_args` on empty-input `continue`, builtin `continue`, and post-`waitpid` paths to eliminate per-command leaks.
- [2026-03-28] - Restored safe `realloc` failure handling - In `_parse_command_line` (`src/parse.c`), allocation checks now validate `temp` before assignment so allocation failure does not lead to NULL dereference.
- [2026-03-28] - Expanded parser delimiters to whitespace - In `_parse_command_line` (`src/parse.c`), delimiters changed from `" "` to `" \t\r\n\v\f"` so `getline` trailing newline does not remain in command tokens.
- [2026-03-28] - Unified parser type usage through header - `src/parse.c` now includes `parse.h` and uses `CmdArgs` from the shared interface instead of a local duplicate struct definition.
- [2026-03-28] - Added `_free_cmd_args` implementation - In `src/parse.c`, added cleanup that frees every token, frees `args_arr`, and resets fields to a known empty state.
- [2026-03-28] - Updated parser behavior tests - In `tests/test_parse.c`, replaced the old non-empty-input `exit(1)` expectation with successful parse assertions and cleanup checks.
- [2026-03-28] - Added newline-tokenization regression test - In `tests/test_parse.c`, added coverage for `"exit\n"` parsing to ensure single-token commands are normalized before builtin/exec paths.
- [2026-03-28] - Revalidated test targets - Ran `make test` (both `test_parse` and `test_builtins` passed) after parser/shell fixes.
