# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Initial `xsh` shell scaffold in `src/`, including the entrypoint, shell loop, command parsing, and builtin dispatch hooks.
- `Makefile` targets for building the shell binary and running unit tests.
- Parser and builtin unit tests in `tests/`.
- GitHub Actions workflows for build and test validation on pushes and pull requests.

### Changed
- Updated `.gitignore` to exclude build artifacts and swap files.

### Notes
- `TODO.md` records current review findings and follow-up issues in the parser and shell execution flow.
