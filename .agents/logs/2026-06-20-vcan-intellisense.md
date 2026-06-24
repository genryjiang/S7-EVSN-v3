# 2026-06-20 - vcan-intellisense

## Actions

- Added `vcan-drivers/dev-can-linux` to the Python EVSN CLI compile-command
  editor flags.
- Added the same include root to the legacy Node compile-command rewriter.
- Extended the compile-command rewrite unit test to assert the generated host
  compile database contains the `vcan-drivers/dev-can-linux` include root.

## Verification

- `python3 -m py_compile tools/evsn_cli/*.py` passed.
- `node --check scripts/rewrite-compile-commands.mjs` passed.
- `python3 -m unittest discover -s tests/evsn_cli` passed with 14 tests.
- `./scripts/evsn compile-commands --dry-run` passed.
- `./init.sh check` passed, validating 18 work items and links in 97 Markdown
  files.
- `git diff --check` passed.
