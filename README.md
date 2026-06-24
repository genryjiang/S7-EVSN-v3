# S7-EVSN-v3

DDS-based control system for SDVs. Uses RTI Connext running on QNX for vehicle
control and isolated processes for drive systems control. This code is intended
to be deployed and run on a Jacinto 7 based TDA4x CoM supported board.

This codebase is intended to be a framework in which you are able to create your own embedded software networks. For reference, I have ported over a version of the software network of a car that was built by a student project I was apart of: Sunswift Racing 7 (albeit heavily modified as to not release the source code).

## Setup

This repo includes also AWS infrastructure for developer environment building,
(soon to be added) optional CI and deployment pipelines, and a (soon to be added) provisioned cloud instance of QNX. Said infrastructure can be found in `cloud-infra`.

## EVSN QNX CLI

The preferred repo-local build entrypoint is:

```bash
./scripts/evsn help
```

Common commands:

```bash
./scripts/evsn doctor --offline
./scripts/evsn build --dry-run --target evsn_node
./scripts/evsn build --target evsn_node
./scripts/evsn compile-commands
./scripts/evsn clean --build-cache --artifacts --yes
./scripts/evsn shell
```

QNX builds use the Docker builder configured by `docker/compose.yml` and
`docker/.env`. Copy `docker/.env.example` to `docker/.env` and set
`DEV_ENV_BUILDER_IMAGE` before running non-dry-run QNX build commands.

EVSN is QNX-only. It uses CMake with the Ninja generator, installs QNX
artifacts under `out/qnx`, and does not require a repository marker file.

Legacy `scripts/qnx-*` wrappers remain available, but delegate to
`./scripts/evsn`.

## License

This repository is proprietary and confidential. You may not use, copy, modify,
distribute, deploy, or otherwise operate this codebase unless you have express
prior written permission from the copyright holder or an authorized
representative. See `LICENSE`.
