from __future__ import annotations

import io
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT / "tools"))

from evsn_cli import cli
from evsn_cli.compile_commands import rewrite_compile_commands


def make_repo(root: Path, *, env_file: bool = False) -> Path:
    (root / "cmake").mkdir(parents=True)
    (root / "docker").mkdir(parents=True)
    (root / "evsn-runtime").mkdir(parents=True)
    (root / "CMakeLists.txt").write_text("cmake_minimum_required(VERSION 3.20)\n", encoding="utf-8")
    (root / "cmake" / "qnx.toolchain.cmake").write_text("set(CMAKE_SYSTEM_NAME QNX)\n", encoding="utf-8")
    (root / "docker" / "compose.yml").write_text("services: {}\n", encoding="utf-8")
    (root / "evsn-runtime" / "CMakeLists.txt").write_text("# runtime\n", encoding="utf-8")
    if env_file:
        (root / "docker" / ".env").write_text("DEV_ENV_BUILDER_IMAGE=image\n", encoding="utf-8")
    return root


class RootResolutionTests(unittest.TestCase):
    def test_repo_root_argument_precedes_environment(self) -> None:
        with tempfile.TemporaryDirectory() as first, tempfile.TemporaryDirectory() as second:
            first_repo = make_repo(Path(first))
            second_repo = make_repo(Path(second))

            resolved = cli.resolve_repo_root(
                str(first_repo),
                {"EVSN_REPO_ROOT": str(second_repo)},
                second_repo,
            )

            self.assertEqual(first_repo.resolve(), resolved.root)

    def test_parent_walk_uses_structure_without_marker(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp))
            nested = repo / "a" / "b"
            nested.mkdir(parents=True)

            resolved = cli.resolve_repo_root(None, {}, nested)

            self.assertEqual(repo.resolve(), resolved.root)


class DryRunPlanTests(unittest.TestCase):
    def test_build_dry_run_does_not_need_docker_or_env_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp), env_file=False)
            stdout = io.StringIO()
            stderr = io.StringIO()

            code = cli.main(
                ["build", "--dry-run", "--target", "z_target", "--target", "a_target", "--repo-root", str(repo)],
                stdout=stdout,
                stderr=stderr,
                env={},
                cwd=repo,
            )

            self.assertEqual(0, code, stderr.getvalue())
            output = stdout.getvalue()
            self.assertIn("docker compose --project-name s7-evsn-qnx", output)
            self.assertNotIn("--env-file", output)
            self.assertIn("evsn-qnx Release 8 a_target z_target", output)
            self.assertIn("PYTHONPATH=", output)

    def test_build_dry_run_uses_docker_env_file_when_present(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp), env_file=True)
            stdout = io.StringIO()

            code = cli.main(
                ["--repo-root", str(repo), "build", "--dry-run", "--target", "evsn_node"],
                stdout=stdout,
                stderr=io.StringIO(),
                env={},
                cwd=repo,
            )

            self.assertEqual(0, code)
            self.assertIn(f"--env-file {(repo / 'docker' / '.env').resolve()}", stdout.getvalue())

    def test_clean_requires_yes(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp))
            stderr = io.StringIO()

            code = cli.main(["--repo-root", str(repo), "clean"], stderr=stderr, env={}, cwd=repo)

            self.assertEqual(2, code)
            self.assertIn("clean requires --yes", stderr.getvalue())

    def test_clean_dry_run_defaults_to_cache_and_artifacts(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp))
            stdout = io.StringIO()

            code = cli.main(
                ["--repo-root", str(repo), "clean", "--dry-run"],
                stdout=stdout,
                stderr=io.StringIO(),
                env={},
                cwd=repo,
            )

            self.assertEqual(0, code)
            output = stdout.getvalue()
            self.assertIn("docker volume rm dev-env-builder-qnx-build-amd64", output)
            self.assertIn(f"find {(repo / 'out' / 'qnx').resolve()}", output)

    def test_shell_dry_run_plans_interactive_builder_shell(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp))
            stdout = io.StringIO()

            code = cli.main(
                ["--repo-root", str(repo), "shell", "--dry-run"],
                stdout=stdout,
                stderr=io.StringIO(),
                env={},
                cwd=repo,
            )

            self.assertEqual(0, code)
            output = stdout.getvalue()
            self.assertIn("exec --user developer dev-env-builder bash", output)
            self.assertNotIn("bash -lc ''", output)

    def test_build_script_filters_rti_arches_to_qnx_names(self) -> None:
        script = cli._build_script()

        self.assertIn('grep -Ei "qnx|nto"', script)
        self.assertNotIn("QOS", script)


class CleanGuardTests(unittest.TestCase):
    def test_artifact_clean_refuses_symlink_outside_repo(self) -> None:
        with tempfile.TemporaryDirectory() as tmp, tempfile.TemporaryDirectory() as outside:
            repo = make_repo(Path(tmp))
            out = repo / "out"
            out.mkdir()
            (out / "qnx").symlink_to(Path(outside), target_is_directory=True)

            with self.assertRaises(cli.EVSNError):
                cli._clean_artifacts(cli.Repo(repo.resolve()))


class ArtifactVerificationTests(unittest.TestCase):
    def test_verify_requires_executable_artifacts(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = cli.Repo(make_repo(Path(tmp)).resolve())

            with self.assertRaises(cli.EVSNError):
                cli._artifact_inventory(repo, require=True)

    def test_artifact_verification_ignores_qnx_in_path_name(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo_path = make_repo(Path(tmp))
            artifact = repo_path / "out" / "qnx" / "bin" / "linux_tool"
            artifact.parent.mkdir(parents=True)
            artifact.write_text("binary", encoding="utf-8")
            artifact.chmod(0o755)
            completed = mock.Mock()
            completed.returncode = 0
            completed.stdout = f"{artifact}: ELF 64-bit LSB executable, x86-64\n"
            completed.stderr = ""

            with mock.patch.object(cli.shutil, "which", return_value="/usr/bin/file"):
                with mock.patch.object(cli.subprocess, "run", return_value=completed):
                    with self.assertRaises(cli.EVSNError):
                        cli._artifact_inventory(cli.Repo(repo_path.resolve()), require=True)


class EnvironmentValidationTests(unittest.TestCase):
    def test_empty_env_file_is_not_a_builder_image_source(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo_path = make_repo(Path(tmp), env_file=True)
            (repo_path / "docker" / ".env").write_text("# missing image\n", encoding="utf-8")

            self.assertFalse(cli._has_builder_image_source(cli.Repo(repo_path.resolve()), {}))

    def test_env_var_can_supply_builder_image_source(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo_path = make_repo(Path(tmp), env_file=False)

            self.assertTrue(
                cli._has_builder_image_source(
                    cli.Repo(repo_path.resolve()),
                    {"DEV_ENV_BUILDER_IMAGE": "example/image:tag"},
                )
            )


class CompileCommandRewriteTests(unittest.TestCase):
    def test_rewrite_compile_commands_replaces_container_paths_and_qcc_syntax(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            repo = make_repo(Path(tmp))
            output_dir = repo / "out" / "qnx"
            output_dir.mkdir(parents=True)
            (output_dir / "compile_commands.container.json").write_text(
                json.dumps(
                    [
                        {
                            "directory": "/workspace/project/build",
                            "file": "/workspace/project/evsn-runtime/evsn_node/evsn_node.cpp",
                            "command": "/workspace/project/scripts/qnx-aarch64-g++ -Y_cxx -Vgcc_ntoaarch64le -I/workspace/project/evsn-runtime/evsn_node -c /workspace/project/evsn-runtime/evsn_node/evsn_node.cpp",
                        }
                    ]
                ),
                encoding="utf-8",
            )

            output = rewrite_compile_commands(repo)
            rewritten = output.read_text(encoding="utf-8")

            self.assertIn('"directory": "' + str(repo.resolve()) + '"', rewritten)
            self.assertIn("clang++", rewritten)
            self.assertNotIn("qnx-aarch64-g++", rewritten)
            self.assertNotIn("-Y_cxx", rewritten)
            self.assertIn(str(repo.resolve() / "vcan-drivers" / "dev-can-linux"), rewritten)


if __name__ == "__main__":
    unittest.main()
