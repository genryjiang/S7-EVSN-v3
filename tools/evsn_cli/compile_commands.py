"""Host-side compile database rewriting for EVSN QNX builds."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any, Iterable


QCC_ONLY_PATTERNS = (
    re.compile(r"^-Y_cxx$"),
    re.compile(r"^-V.+"),
    re.compile(r"^-Wc,-isysroot,.+"),
    re.compile(r"^-lang-c\+\+$"),
)

FORBIDDEN_COMMAND_PATTERNS = (
    re.compile(r"\s-Y_cxx(?=\s|\")"),
    re.compile(r"qnx-aarch64-g\+\+"),
    re.compile(r"qnx-aarch64-gcc"),
    re.compile(r"\sq\+\+(?=\s|\")"),
    re.compile(r"\sqcc(?=\s|\")"),
)


def _editor_flags(repo_root: Path) -> list[str]:
    qnx_root = repo_root / ".intellisense" / "qnx800"
    rti_root = repo_root / ".intellisense" / "rti_connext_dds-7.3.1"
    return [
        "-I",
        str(repo_root / "evsn-runtime" / "dds_types" / "std_types"),
        "-I",
        str(repo_root / "evsn-runtime" / "dds_types" / "custom_types"),
        "-I",
        str(repo_root / "evsn-runtime" / "evsn_node"),
        "-I",
        str(repo_root / "vcan-drivers" / "dev-can-linux"),
        "--target=aarch64-unknown-nto",
        "-isystem",
        str(qnx_root / "target/qnx/usr/include/c++/v1"),
        "-isystem",
        str(qnx_root / "host/linux/x86_64/usr/lib/gcc/aarch64-unknown-nto-qnx8.0.0/12.2.0/include"),
        "-isystem",
        str(qnx_root / "target/qnx/usr/include"),
        "-isystem",
        str(qnx_root / "target/qnx/usr/local/include"),
        "-isystem",
        str(rti_root / "include"),
        "-isystem",
        str(rti_root / "include/ndds"),
        "-isystem",
        str(rti_root / "include/ndds/hpp"),
        "-D__QNXNTO__=1",
        "-D__QNX__=800",
        "-D__LITTLEENDIAN__=1",
        "-DQNX_LIBM_BUILTINS=1",
    ]


def _rewrite_string(value: str, repo_root: Path) -> str:
    replacements = (
        ("/home/developer/rti_connext_dds-7.3.1", str(repo_root / ".intellisense/rti_connext_dds-7.3.1")),
        ("/home/developer/qnx800", str(repo_root / ".intellisense/qnx800")),
        ("/workspace/project", str(repo_root)),
    )
    result = value
    for container_path, host_path in replacements:
        result = result.replace(container_path, host_path)
    return result


def _rewrite_value(value: Any, repo_root: Path) -> Any:
    if isinstance(value, str):
        return _rewrite_string(value, repo_root)
    if isinstance(value, list):
        return [_rewrite_value(item, repo_root) for item in value]
    if isinstance(value, dict):
        return {key: _rewrite_value(item, repo_root) for key, item in value.items()}
    return value


def _strip_qcc_only_flags(command: str) -> str:
    command = re.sub(r"\s-Y_cxx(?=\s|$)", "", command)
    command = re.sub(r"\s-V\S+", "", command)
    command = re.sub(r"\s-Wc,-isysroot,\S+", "", command)
    command = re.sub(r"\s-lang-c\+\+", "", command)
    return command


def _rewrite_compiler(command: str) -> str:
    command = re.sub(r"^\S*(?:q\+\+|qnx-aarch64-g\+\+)\s+", "clang++ ", command)
    command = re.sub(r"^\S*(?:qcc|qnx-aarch64-gcc)\s+", "clang ", command)
    return command


def _append_editor_flags(command: str, flags: Iterable[str]) -> str:
    encoded_flags = " ".join(json.dumps(flag) for flag in flags)
    return f"{command} {encoded_flags}"


def _is_qcc_only_flag(argument: str) -> bool:
    return any(pattern.match(argument) for pattern in QCC_ONLY_PATTERNS)


def _arguments_to_command(arguments: list[Any], flags: list[str]) -> str | None:
    if not arguments:
        return None

    compiler = str(arguments[0]).rsplit("/", 1)[-1]
    editor_compiler = "clang++" if "++" in compiler else "clang"
    rewritten = [editor_compiler]
    rewritten.extend(str(argument) for argument in arguments[1:] if not _is_qcc_only_flag(str(argument)))
    rewritten.extend(flags)
    return " ".join(json.dumps(argument) for argument in rewritten)


def rewrite_compile_commands(repo_root: Path) -> Path:
    """Rewrite the container QNX compile database into a host clangd database."""

    repo_root = repo_root.resolve()
    input_path = repo_root / "out" / "qnx" / "compile_commands.container.json"
    output_path = repo_root / "out" / "qnx" / "compile_commands.json"
    if not input_path.is_file() or input_path.stat().st_size == 0:
        raise FileNotFoundError(f"missing container compile database: {input_path}")

    database = json.loads(input_path.read_text(encoding="utf-8"))
    if not isinstance(database, list):
        raise ValueError(f"compile database must be a JSON array: {input_path}")

    flags = _editor_flags(repo_root)
    host_database: list[dict[str, Any]] = []
    for raw_entry in database:
        if not isinstance(raw_entry, dict):
            raise ValueError("compile database entries must be JSON objects")

        entry = _rewrite_value(raw_entry, repo_root)
        command = entry.get("command")
        arguments = entry.get("arguments")

        output_entry = dict(entry)
        output_entry["directory"] = str(repo_root)
        output_entry.pop("arguments", None)

        if isinstance(command, str):
            rewritten_command = _strip_qcc_only_flags(_rewrite_compiler(command))
            output_entry["command"] = _append_editor_flags(rewritten_command, flags)
        elif isinstance(arguments, list):
            rewritten_from_args = _arguments_to_command(arguments, flags)
            if rewritten_from_args is not None:
                output_entry["command"] = rewritten_from_args
        else:
            raise ValueError("compile database entry must contain command or arguments")

        host_database.append(output_entry)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(f"{json.dumps(host_database, indent=2)}\n", encoding="utf-8")
    output_text = output_path.read_text(encoding="utf-8")
    for pattern in FORBIDDEN_COMMAND_PATTERNS:
        if pattern.search(output_text):
            raise ValueError(f"generated compile database still contains QNX-only syntax: {pattern.pattern}")

    return output_path


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Rewrite EVSN QNX compile commands for host tooling.")
    parser.add_argument("--repo-root", required=True)
    args = parser.parse_args(argv)
    output_path = rewrite_compile_commands(Path(args.repo_root))
    print(f"Updated {output_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
