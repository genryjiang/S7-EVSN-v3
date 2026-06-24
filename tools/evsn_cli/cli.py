"""Clean-sheet EVSN QNX-only build CLI."""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import textwrap
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Sequence, TextIO

from .compile_commands import rewrite_compile_commands


REQUIRED_REPO_FILES = (
    "CMakeLists.txt",
    "cmake/qnx.toolchain.cmake",
    "docker/compose.yml",
    "evsn-runtime/CMakeLists.txt",
)
DEFAULT_COMPOSE_PROJECT = "s7-evsn-qnx"
DEFAULT_BUILDER_SERVICE = "dev-env-builder"
DEFAULT_BUILD_TYPE = "Release"
DEFAULT_BUILD_JOBS = 8
QNX_BUILD_VOLUME = "dev-env-builder-qnx-build-amd64"


class EVSNError(RuntimeError):
    """User-facing CLI error."""


@dataclass(frozen=True)
class Command:
    argv: tuple[str, ...]
    cwd: Path | None = None
    env: Mapping[str, str] | None = None

    def render(self) -> str:
        command = " ".join(_shell_quote(part) for part in self.argv)
        if self.env:
            assignments = " ".join(f"{key}={_shell_quote(value)}" for key, value in sorted(self.env.items()))
            command = f"{assignments} {command}"
        if self.cwd is not None:
            command = f"cd {_shell_quote(str(self.cwd))} && {command}"
        return command


@dataclass(frozen=True)
class Repo:
    root: Path

    @property
    def compose_file(self) -> Path:
        return self.root / "docker" / "compose.yml"

    @property
    def env_file(self) -> Path:
        return self.root / "docker" / ".env"

    @property
    def out_qnx(self) -> Path:
        return self.root / "out" / "qnx"

    @property
    def tools_dir(self) -> Path:
        return self.root / "tools"


@dataclass(frozen=True)
class DockerBackend:
    repo: Repo
    project: str
    service: str

    def compose_base(self) -> list[str]:
        argv = ["docker", "compose", "--project-name", self.project]
        if self.repo.env_file.is_file():
            argv.extend(["--env-file", str(self.repo.env_file)])
        argv.extend(["-f", str(self.repo.compose_file)])
        return argv

    def ensure_builder_command(self) -> Command:
        return Command(tuple(self.compose_base() + ["up", "-d", self.service]))

    def exec_bash_command(self, script: str, args: Sequence[str] = (), *, tty: bool = False) -> Command:
        exec_args = ["exec"]
        if not tty:
            exec_args.append("-T")
        exec_args.extend(["--user", "developer", self.service, "bash", "-lc", script, "evsn-qnx"])
        exec_args.extend(args)
        return Command(tuple(self.compose_base() + exec_args))

    def stop_builder_command(self) -> Command:
        return Command(tuple(self.compose_base() + ["stop", self.service]))

    def remove_builder_command(self) -> Command:
        return Command(tuple(self.compose_base() + ["rm", "-f", self.service]))

    def remove_build_volume_command(self) -> Command:
        return Command(("docker", "volume", "rm", QNX_BUILD_VOLUME))

    def shell_command(self) -> Command:
        return Command(tuple(self.compose_base() + ["exec", "--user", "developer", self.service, "bash"]))


def _shell_quote(value: str) -> str:
    import shlex

    return shlex.quote(value)


def _positive_int(value: str) -> int:
    try:
        parsed = int(value, 10)
    except ValueError as exc:
        raise argparse.ArgumentTypeError("must be an integer") from exc
    if parsed < 1:
        raise argparse.ArgumentTypeError("must be at least 1")
    return parsed


def _extract_repo_root_arg(argv: Sequence[str]) -> tuple[list[str], str | None]:
    remaining: list[str] = []
    repo_root: str | None = None
    index = 0
    while index < len(argv):
        arg = argv[index]
        if arg == "--repo-root":
            index += 1
            if index >= len(argv):
                raise EVSNError("--repo-root requires a value")
            repo_root = argv[index]
        elif arg.startswith("--repo-root="):
            repo_root = arg.split("=", 1)[1]
            if not repo_root:
                raise EVSNError("--repo-root requires a value")
        else:
            remaining.append(arg)
        index += 1
    return remaining, repo_root


def _build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="evsn",
        description="EVSN QNX-only build CLI.",
        epilog="Global option: --repo-root PATH may be placed before or after the command.",
    )
    subparsers = parser.add_subparsers(dest="command")

    subparsers.add_parser("help", help="Show this help text.")

    doctor = subparsers.add_parser("doctor", help="Check EVSN QNX build prerequisites.")
    doctor.add_argument("--offline", action="store_true", help="Skip Docker daemon checks.")

    build = subparsers.add_parser("build", help="Build EVSN QNX artifacts.")
    build.add_argument("--dry-run", action="store_true", help="Print the command plan without executing it.")
    build.add_argument("--target", action="append", default=[], help="CMake target to build. May be repeated.")
    build.add_argument("--jobs", type=_positive_int, default=DEFAULT_BUILD_JOBS, help="Parallel build jobs.")
    build.add_argument("--build-type", default=DEFAULT_BUILD_TYPE, help="CMake build type.")

    clean = subparsers.add_parser("clean", help="Clean QNX build cache and/or artifacts.")
    clean.add_argument("--dry-run", action="store_true", help="Print the clean plan without deleting anything.")
    clean.add_argument("--build-cache", action="store_true", help="Remove the Docker QNX build cache volume.")
    clean.add_argument("--artifacts", action="store_true", help="Remove generated files under out/qnx.")
    clean.add_argument("--yes", action="store_true", help="Required acknowledgement for deletion.")

    shell = subparsers.add_parser("shell", help="Open an interactive shell in the QNX builder.")
    shell.add_argument("--dry-run", action="store_true", help="Print the command plan without opening a shell.")

    compile_commands = subparsers.add_parser(
        "compile-commands",
        help="Refresh host compile_commands.json from the QNX build cache.",
    )
    compile_commands.add_argument("--dry-run", action="store_true", help="Print the command plan without executing it.")

    verify = subparsers.add_parser("verify", help="Build and inspect QNX artifacts.")
    verify.add_argument("--dry-run", action="store_true", help="Print the verification plan without executing it.")
    verify.add_argument("--target", action="append", default=[], help="CMake target to verify. May be repeated.")

    return parser


def _is_structural_repo(path: Path) -> bool:
    return all((path / item).is_file() for item in REQUIRED_REPO_FILES)


def validate_repo_root(path: Path) -> Repo:
    root = path.expanduser().resolve()
    missing = [item for item in REQUIRED_REPO_FILES if not (root / item).is_file()]
    if missing:
        details = "\n".join(f"  - {item}" for item in missing)
        raise EVSNError(f"{root} is not an EVSN repository; missing:\n{details}")
    return Repo(root=root)


def _git_toplevel(cwd: Path) -> Path | None:
    try:
        completed = subprocess.run(
            ["git", "rev-parse", "--show-toplevel"],
            cwd=str(cwd),
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            check=False,
        )
    except OSError:
        return None
    if completed.returncode != 0:
        return None
    output = completed.stdout.strip()
    return Path(output) if output else None


def _parent_walk(cwd: Path) -> Path | None:
    current = cwd.expanduser().resolve()
    for candidate in (current, *current.parents):
        if _is_structural_repo(candidate):
            return candidate
    return None


def resolve_repo_root(repo_root_arg: str | None, env: Mapping[str, str], cwd: Path) -> Repo:
    if repo_root_arg:
        return validate_repo_root(Path(repo_root_arg))

    env_root = env.get("EVSN_REPO_ROOT")
    if env_root:
        return validate_repo_root(Path(env_root))

    git_root = _git_toplevel(cwd)
    if git_root is not None and _is_structural_repo(git_root.resolve()):
        return validate_repo_root(git_root)

    parent_root = _parent_walk(cwd)
    if parent_root is not None:
        return validate_repo_root(parent_root)

    raise EVSNError("could not resolve EVSN repo root; pass --repo-root or set EVSN_REPO_ROOT")


def _backend(repo: Repo, env: Mapping[str, str]) -> DockerBackend:
    project = env.get("EVSN_COMPOSE_PROJECT") or env.get("QNX_COMPOSE_PROJECT") or DEFAULT_COMPOSE_PROJECT
    service = env.get("EVSN_BUILDER_SERVICE") or env.get("QNX_BUILDER_SERVICE") or DEFAULT_BUILDER_SERVICE
    return DockerBackend(repo=repo, project=project, service=service)


def _validate_name_list(values: Sequence[str], label: str) -> list[str]:
    cleaned: list[str] = []
    pattern = re.compile(r"^[A-Za-z0-9_.:+-]+$")
    for value in values:
        if not value or not pattern.match(value):
            raise EVSNError(f"invalid {label}: {value!r}")
        cleaned.append(value)
    return sorted(set(cleaned))


def _validate_build_type(value: str) -> str:
    if not value or not re.match(r"^[A-Za-z0-9_.:+-]+$", value):
        raise EVSNError(f"invalid build type: {value!r}")
    return value


def _build_script() -> str:
    return textwrap.dedent(
        r"""
        set -euo pipefail

        EVSN_BUILD_TYPE="${1:?missing build type}"
        shift
        EVSN_BUILD_JOBS="${1:?missing build jobs}"
        shift
        targets=("$@")

        [[ "$(uname -m)" == "x86_64" ]] || {
          echo "Refusing QNX build: builder container must run as x86_64." >&2
          exit 1
        }
        : "${QNX_HOST:?QNX_HOST is not set in the builder}"
        : "${QNX_TARGET:?QNX_TARGET is not set in the builder}"
        : "${NDDSHOME:?NDDSHOME is not set in the builder}"
        [[ -x "${QNX_HOST}/usr/bin/aarch64-unknown-nto-qnx8.0.0-gcc" ]] && \
          [[ -x "${QNX_HOST}/usr/bin/aarch64-unknown-nto-qnx8.0.0-g++" ]] || {
          echo "QNX AArch64 GCC drivers are missing from the builder image at ${QNX_HOST}." >&2
          exit 1
        }
        [[ -d "${NDDSHOME}" ]] && [[ -n "$(find "${NDDSHOME}" -mindepth 1 -maxdepth 1 -print -quit)" ]] || {
          echo "RTI Connext is missing from the QNX builder image at ${NDDSHOME}." >&2
          exit 1
        }
        mapfile -t qnx_rti_arches < <(
          find "${NDDSHOME}/lib" -mindepth 1 -maxdepth 1 -type d -printf "%f\n" \
            | grep -Ei "qnx|nto" || true
        )
        if [[ -n "${CONNEXTDDS_ARCH:-}" && "${CONNEXTDDS_ARCH}" == *QNX* ]]; then
          [[ -d "${NDDSHOME}/lib/${CONNEXTDDS_ARCH}" ]] || {
            echo "Configured CONNEXTDDS_ARCH is not installed: ${CONNEXTDDS_ARCH}" >&2
            exit 1
          }
        elif [[ "${#qnx_rti_arches[@]}" -eq 1 ]]; then
          if [[ -n "${CONNEXTDDS_ARCH:-}" ]]; then
            echo "Ignoring non-QNX CONNEXTDDS_ARCH for QNX build: ${CONNEXTDDS_ARCH}" >&2
          fi
          export CONNEXTDDS_ARCH="${qnx_rti_arches[0]}"
        elif [[ "${#qnx_rti_arches[@]}" -eq 0 ]]; then
          echo "The builder image has no RTI QNX target libraries under ${NDDSHOME}/lib." >&2
          echo "Publish an RTI archive containing both Linux and QNX target libraries." >&2
          exit 1
        else
          printf "Multiple RTI QNX architectures are installed; set CONNEXTDDS_ARCH. Choices:\n" >&2
          printf "  %s\n" "${qnx_rti_arches[@]}" >&2
          exit 1
        fi
        [[ -f /workspace/project/cmake/qnx.toolchain.cmake ]] || {
          echo "Missing QNX toolchain file at /workspace/project/cmake/qnx.toolchain.cmake. Refusing to produce Linux artifacts as QNX artifacts." >&2
          exit 1
        }

        export PATH="${QNX_HOST}/usr/bin:${QNX_HOST}/usr/sbin:${PATH}"
        export MAKEFLAGS="-I${QNX_TARGET}/usr/include"
        export CMAKE_TOOLCHAIN_FILE=/workspace/project/cmake/qnx.toolchain.cmake
        export LD_LIBRARY_PATH="${LD_LIBRARY_PATH:-}"
        export PYTHONDONTWRITEBYTECODE=1
        unset PYTHONPATH || true

        cd /workspace/project
        if [[ -f build/CMakeCache.txt ]]; then
          cmake_system_file="$(find build/CMakeFiles -path "*/CMakeSystem.cmake" -print -quit 2>/dev/null || true)"
          if [[ -z "${cmake_system_file}" ]] || ! grep -q "set(CMAKE_SYSTEM \"QNX" "${cmake_system_file}"; then
            echo "Existing build volume was not configured for QNX. Run ./scripts/evsn clean --build-cache --yes first." >&2
            exit 1
          fi
        fi

        cmake_args=(
          -S /workspace/project
          -B /workspace/project/build
          -G Ninja
          -DCMAKE_BUILD_TYPE="${EVSN_BUILD_TYPE}"
          -DCMAKE_TOOLCHAIN_FILE=/workspace/project/cmake/qnx.toolchain.cmake
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
          -DCMAKE_INSTALL_PREFIX=/workspace/project/deploy
          -DBUILD_TESTING=OFF
        )
        cmake "${cmake_args[@]}"

        build_args=(--parallel "${EVSN_BUILD_JOBS}")
        if [[ "${#targets[@]}" -gt 0 ]]; then
          build_args+=(--target "${targets[@]}")
        fi
        cmake --build /workspace/project/build "${build_args[@]}"

        mkdir -p /workspace/project/deploy
        find /workspace/project/deploy -mindepth 1 -maxdepth 1 ! -name .gitkeep -exec rm -rf -- {} +
        cmake --install /workspace/project/build

        grep -q "set(CMAKE_SYSTEM \"QNX" /workspace/project/build/CMakeFiles/*/CMakeSystem.cmake || {
          echo "Build completed without a QNX CMake system. Refusing artifacts." >&2
          exit 1
        }
        grep -R -Eq "CMAKE_(C|CXX)_COMPILER \".*/qnx-aarch64-g(\+\+|cc)\"" \
          /workspace/project/build/CMakeFiles/*/CMake*Compiler.cmake || {
          echo "Build completed without a QNX compiler. Refusing artifacts." >&2
          exit 1
        }
        if [[ -d /workspace/project/deploy/bin ]]; then
          while IFS= read -r -d "" artifact; do
            description="$(file "${artifact}")"
            echo "${description}"
            lower_description="${description,,}"
            if [[ "${description}" == *"ELF"* && "${lower_description}" != *"qnx"* && "${lower_description}" != *"nto"* ]]; then
              echo "Installed executable is not identified as a QNX artifact: ${artifact}" >&2
              exit 1
            fi
          done < <(find /workspace/project/deploy/bin -type f -perm -111 -print0)
        fi
        """
    ).strip()


def _compile_commands_copy_script() -> str:
    return textwrap.dedent(
        r"""
        set -euo pipefail
        source_file=/workspace/project/build/compile_commands.json
        target_file=/workspace/project/deploy/compile_commands.container.json

        [[ -s "${source_file}" ]] || {
          echo "No compile_commands.json exists in the QNX build volume." >&2
          exit 1
        }
        cmake_system_file="$(find /workspace/project/build/CMakeFiles -path "*/CMakeSystem.cmake" -print -quit 2>/dev/null || true)"
        [[ -n "${cmake_system_file}" ]] && grep -q "set(CMAKE_SYSTEM \"QNX" "${cmake_system_file}" || {
          echo "Existing compile database was not produced by a QNX CMake configure." >&2
          exit 1
        }

        mkdir -p "$(dirname "${target_file}")"
        cp "${source_file}" "${target_file}"
        test -s "${target_file}"
        echo "Updated out/qnx/compile_commands.container.json"
        """
    ).strip()


def _python_rewrite_command(repo: Repo) -> Command:
    env = {"PYTHONPATH": str(repo.tools_dir)}
    return Command(
        ("python3", "-m", "evsn_cli.compile_commands", "--repo-root", str(repo.root)),
        cwd=repo.root,
        env=env,
    )


def _build_plan(repo: Repo, backend: DockerBackend, *, targets: Sequence[str], jobs: int, build_type: str) -> list[Command]:
    args = [build_type, str(jobs), *targets]
    return [
        backend.ensure_builder_command(),
        backend.exec_bash_command(_build_script(), args),
        backend.exec_bash_command(_compile_commands_copy_script()),
        _python_rewrite_command(repo),
    ]


def _compile_commands_plan(repo: Repo, backend: DockerBackend) -> list[Command]:
    return [
        backend.ensure_builder_command(),
        backend.exec_bash_command(_compile_commands_copy_script()),
        _python_rewrite_command(repo),
    ]


def _shell_plan(backend: DockerBackend) -> list[Command]:
    return [
        backend.ensure_builder_command(),
        backend.shell_command(),
    ]


def _clean_build_cache_plan(backend: DockerBackend) -> list[Command]:
    return [
        backend.stop_builder_command(),
        backend.remove_builder_command(),
        backend.remove_build_volume_command(),
    ]


def _clean_artifacts_render(repo: Repo) -> Command:
    return Command(
        (
            "find",
            str(repo.out_qnx),
            "-mindepth",
            "1",
            "-maxdepth",
            "1",
            "!",
            "-name",
            ".gitkeep",
            "-exec",
            "rm",
            "-rf",
            "--",
            "{}",
            "+",
        )
    )


def _print_plan(commands: Sequence[Command], stdout: TextIO) -> None:
    for command in commands:
        print(command.render(), file=stdout)


def _run_command(command: Command) -> None:
    env = os.environ.copy()
    if command.env:
        env.update(command.env)
    try:
        subprocess.run(
            list(command.argv),
            cwd=str(command.cwd) if command.cwd else None,
            env=env,
            check=True,
        )
    except FileNotFoundError as exc:
        raise EVSNError(f"missing executable: {command.argv[0]}") from exc
    except subprocess.CalledProcessError as exc:
        rendered = command.render()
        raise EVSNError(f"command failed with exit code {exc.returncode}: {rendered}") from exc


def _run_plan(commands: Sequence[Command]) -> None:
    for command in commands:
        if command.argv[:3] == ("python3", "-m", "evsn_cli.compile_commands"):
            repo_index = command.argv.index("--repo-root") + 1
            output = rewrite_compile_commands(Path(command.argv[repo_index]))
            print(f"Updated {output}")
            continue
        _run_command(command)


def _write_transcript(
    repo: Repo,
    *,
    command_name: str,
    commands: Sequence[Command],
    result: str,
    exit_code: int,
    details: Mapping[str, object],
) -> Path:
    repo.out_qnx.mkdir(parents=True, exist_ok=True)
    transcript_path = repo.out_qnx / "evsn-build-transcript.json"
    payload = {
        "command": command_name,
        "repo_root": str(repo.root),
        "backend": "docker",
        "result": result,
        "exit_code": exit_code,
        "steps": [command.render() for command in commands],
        **dict(details),
    }
    transcript_path.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return transcript_path


def _read_env_file_value(path: Path, key: str) -> str | None:
    if not path.is_file():
        return None
    for raw_line in path.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        name, value = line.split("=", 1)
        if name.strip() == key:
            value = value.strip().strip("\"'")
            return value or None
    return None


def _has_builder_image_source(repo: Repo, env: Mapping[str, str]) -> bool:
    return bool(env.get("DEV_ENV_BUILDER_IMAGE") or _read_env_file_value(repo.env_file, "DEV_ENV_BUILDER_IMAGE"))


def _require_docker() -> None:
    if shutil.which("docker") is None:
        raise EVSNError("Docker CLI is required.")
    try:
        completed = subprocess.run(
            ["docker", "info"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
    except OSError as exc:
        raise EVSNError("Docker daemon is not reachable.") from exc
    if completed.returncode != 0:
        raise EVSNError("Docker daemon is not reachable.")


def _require_builder_image_source(repo: Repo, env: Mapping[str, str]) -> None:
    if _has_builder_image_source(repo, env):
        return
    raise EVSNError(
        f"Missing {repo.env_file}. Copy docker/.env.example to docker/.env and set DEV_ENV_BUILDER_IMAGE, "
        "or export DEV_ENV_BUILDER_IMAGE."
    )


def _prepare_docker_execution(repo: Repo, env: Mapping[str, str]) -> None:
    _require_docker()
    _require_builder_image_source(repo, env)


def _guard_out_qnx(repo: Repo) -> Path:
    expected = repo.root / "out" / "qnx"
    if repo.out_qnx != expected:
        raise EVSNError(f"refusing to clean unexpected output path: {repo.out_qnx}")

    root_resolved = repo.root.resolve()
    output_resolved = repo.out_qnx.resolve(strict=False)
    try:
        output_resolved.relative_to(root_resolved)
    except ValueError as exc:
        raise EVSNError(f"refusing to clean output path outside repo: {repo.out_qnx}") from exc
    return repo.out_qnx


def _clean_artifacts(repo: Repo) -> None:
    output_dir = _guard_out_qnx(repo)
    output_dir.mkdir(parents=True, exist_ok=True)
    for child in output_dir.iterdir():
        if child.name == ".gitkeep":
            continue
        if child.is_dir() and not child.is_symlink():
            shutil.rmtree(child)
        else:
            child.unlink()
    print("Removed QNX generated output.")


def _file_description(artifact: Path) -> str:
    if shutil.which("file") is None:
        raise EVSNError("file command is required for artifact verification.")
    completed = subprocess.run(
        ["file", str(artifact)],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if completed.returncode != 0:
        raise EVSNError(f"file failed for {artifact}: {completed.stderr.strip()}")
    stdout = completed.stdout.strip()
    _, separator, description = stdout.partition(": ")
    return description if separator else stdout


def _artifact_inventory(repo: Repo, *, require: bool) -> list[dict[str, object]]:
    bin_dir = repo.out_qnx / "bin"
    if not bin_dir.is_dir():
        if require:
            raise EVSNError(f"no executable artifacts found under {bin_dir}")
        return []
    artifacts = sorted(path for path in bin_dir.iterdir() if path.is_file() and os.access(path, os.X_OK))
    if not artifacts:
        if require:
            raise EVSNError(f"no executable artifacts found under {bin_dir}")
        return []

    inventory: list[dict[str, object]] = []
    for artifact in artifacts:
        description = _file_description(artifact)
        lower_description = description.lower()
        if "ELF" in description and "qnx" not in lower_description and "nto" not in lower_description:
            raise EVSNError(f"artifact is not identified as QNX: {artifact}")
        stat = artifact.stat()
        inventory.append(
            {
                "path": str(artifact),
                "description": description,
                "size": stat.st_size,
                "mtime_ns": stat.st_mtime_ns,
            }
        )
    return inventory


def _verify_artifacts(repo: Repo) -> list[dict[str, object]]:
    inventory = _artifact_inventory(repo, require=True)
    for artifact in inventory:
        print(f"{artifact['path']}: {artifact['description']}")
    return inventory


def _doctor(repo: Repo, *, offline: bool, env: Mapping[str, str], stdout: TextIO) -> int:
    print("EVSN doctor", file=stdout)
    print(f"repo root: {repo.root}", file=stdout)
    print("structure: ok", file=stdout)
    print(f"compose file: {repo.compose_file}", file=stdout)
    if env.get("DEV_ENV_BUILDER_IMAGE"):
        print("docker env: DEV_ENV_BUILDER_IMAGE from environment", file=stdout)
    elif _read_env_file_value(repo.env_file, "DEV_ENV_BUILDER_IMAGE"):
        print(f"docker env: {repo.env_file}", file=stdout)
    else:
        print("docker env: missing DEV_ENV_BUILDER_IMAGE in docker/.env or environment", file=stdout)

    if offline:
        print("docker: skipped (--offline)", file=stdout)
        return 0

    failures = 0
    if shutil.which("docker") is None:
        print("docker: missing Docker CLI", file=stdout)
        failures += 1
    else:
        print("docker: Docker CLI found", file=stdout)
        completed = subprocess.run(["docker", "info"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
        if completed.returncode == 0:
            print("docker daemon: reachable", file=stdout)
        else:
            print("docker daemon: not reachable", file=stdout)
            failures += 1
    if not _has_builder_image_source(repo, env):
        failures += 1
    return 1 if failures else 0


def _handle_build(args: argparse.Namespace, repo: Repo, env: Mapping[str, str], stdout: TextIO) -> int:
    targets = _validate_name_list(args.target, "target")
    build_type = _validate_build_type(args.build_type)
    backend = _backend(repo, env)
    commands = _build_plan(repo, backend, targets=targets, jobs=args.jobs, build_type=build_type)
    if args.dry_run:
        _print_plan(commands, stdout)
        return 0
    _prepare_docker_execution(repo, env)
    details = {
        "build_type": build_type,
        "jobs": args.jobs,
        "targets": targets or ["all"],
        "builder_service": backend.service,
        "compose_project": backend.project,
        "rti_arch": "resolved-in-builder",
    }
    try:
        _run_plan(commands)
        artifacts = _artifact_inventory(repo, require=False)
    except EVSNError:
        _write_transcript(
            repo,
            command_name="build",
            commands=commands,
            result="failed",
            exit_code=2,
            details=details,
        )
        raise
    _write_transcript(
        repo,
        command_name="build",
        commands=commands,
        result="passed",
        exit_code=0,
        details={**details, "artifacts": artifacts},
    )
    return 0


def _handle_clean(args: argparse.Namespace, repo: Repo, env: Mapping[str, str], stdout: TextIO) -> int:
    if not args.yes and not args.dry_run:
        raise EVSNError("clean requires --yes; use --dry-run to preview")

    clean_build_cache = args.build_cache
    clean_artifacts = args.artifacts
    if not clean_build_cache and not clean_artifacts:
        clean_build_cache = True
        clean_artifacts = True

    backend = _backend(repo, env)
    commands: list[Command] = []
    if clean_build_cache:
        commands.extend(_clean_build_cache_plan(backend))
    if clean_artifacts:
        commands.append(_clean_artifacts_render(repo))

    if args.dry_run:
        _print_plan(commands, stdout)
        return 0

    if clean_build_cache:
        _prepare_docker_execution(repo, env)
        for command in _clean_build_cache_plan(backend):
            try:
                _run_command(command)
            except EVSNError as exc:
                if command.argv[:3] == ("docker", "volume", "rm") and "exit code" in str(exc):
                    continue
                raise
    if clean_artifacts:
        _clean_artifacts(repo)
    return 0


def _handle_shell(args: argparse.Namespace, repo: Repo, env: Mapping[str, str], stdout: TextIO) -> int:
    backend = _backend(repo, env)
    commands = _shell_plan(backend)
    if args.dry_run:
        _print_plan(commands, stdout)
        return 0
    _prepare_docker_execution(repo, env)
    _run_plan(commands)
    return 0


def _handle_compile_commands(args: argparse.Namespace, repo: Repo, env: Mapping[str, str], stdout: TextIO) -> int:
    backend = _backend(repo, env)
    commands = _compile_commands_plan(repo, backend)
    if args.dry_run:
        _print_plan(commands, stdout)
        return 0
    _prepare_docker_execution(repo, env)
    _run_plan(commands)
    return 0


def _handle_verify(args: argparse.Namespace, repo: Repo, env: Mapping[str, str], stdout: TextIO) -> int:
    targets = _validate_name_list(args.target, "target")
    backend = _backend(repo, env)
    build_commands = _build_plan(
        repo,
        backend,
        targets=targets,
        jobs=DEFAULT_BUILD_JOBS,
        build_type=DEFAULT_BUILD_TYPE,
    )
    verify_command = Command(
        (
            "find",
            str(repo.out_qnx / "bin"),
            "-type",
            "f",
            "-perm",
            "-111",
            "-exec",
            "file",
            "{}",
            "+",
        )
    )
    if args.dry_run:
        _print_plan([*build_commands, verify_command], stdout)
        return 0
    _prepare_docker_execution(repo, env)
    details = {
        "build_type": DEFAULT_BUILD_TYPE,
        "jobs": DEFAULT_BUILD_JOBS,
        "targets": targets or ["all"],
        "builder_service": backend.service,
        "compose_project": backend.project,
        "rti_arch": "resolved-in-builder",
    }
    try:
        _run_plan(build_commands)
        artifacts = _verify_artifacts(repo)
    except EVSNError:
        _write_transcript(
            repo,
            command_name="verify",
            commands=[*build_commands, verify_command],
            result="failed",
            exit_code=2,
            details=details,
        )
        raise
    _write_transcript(
        repo,
        command_name="verify",
        commands=[*build_commands, verify_command],
        result="passed",
        exit_code=0,
        details={**details, "artifacts": artifacts},
    )
    return 0


def main(
    argv: Sequence[str] | None = None,
    *,
    stdout: TextIO | None = None,
    stderr: TextIO | None = None,
    env: Mapping[str, str] | None = None,
    cwd: Path | None = None,
) -> int:
    stdout = stdout if stdout is not None else sys.stdout
    stderr = stderr if stderr is not None else sys.stderr
    env = env if env is not None else os.environ
    cwd = cwd if cwd is not None else Path.cwd()
    raw_argv = list(sys.argv[1:] if argv is None else argv)

    try:
        parser_argv, repo_root_arg = _extract_repo_root_arg(raw_argv)
        parser = _build_parser()
        if not parser_argv:
            parser_argv = ["help"]
        args = parser.parse_args(parser_argv)
        if args.command in (None, "help"):
            parser.print_help(stdout)
            return 0

        repo = resolve_repo_root(repo_root_arg, env, cwd)
        if args.command == "doctor":
            return _doctor(repo, offline=args.offline, env=env, stdout=stdout)
        if args.command == "build":
            return _handle_build(args, repo, env, stdout)
        if args.command == "clean":
            return _handle_clean(args, repo, env, stdout)
        if args.command == "shell":
            return _handle_shell(args, repo, env, stdout)
        if args.command == "compile-commands":
            return _handle_compile_commands(args, repo, env, stdout)
        if args.command == "verify":
            return _handle_verify(args, repo, env, stdout)

        raise EVSNError(f"unknown command: {args.command}")
    except EVSNError as exc:
        print(f"evsn: {exc}", file=stderr)
        return 2
