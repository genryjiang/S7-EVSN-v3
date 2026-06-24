# Harness Onboarding And Manual Inputs

The harness provides deterministic setup, repository validation, Linux build
and test execution, and optional QNX cross-build verification. It cannot infer
credentials, licensed SDK correctness, target access, priorities, or acceptance
decisions.

## First-Time Use

From the repository root:

```bash
cp docker/.env.example docker/.env
# Fill in the canonical QNX builder ECR image reference.
./init.sh setup
./init.sh verify
```

The setup command initializes submodules, pulls and starts the Compose
services, and refreshes the native editor's read-only QNX/RTI header caches.

## What Maintainers Must Fill In

Maintain `.agents/feature_list.json`. Every real task needs a priority, desired
outcome, checkable acceptance criteria, verification commands, dependencies,
blockers, and durable evidence after checks actually run.

Use these statuses:

- `not_started`: implementation has not begun
- `in_progress`: active work with no external blocker
- `blocked`: cannot proceed; `blockers` must explain why
- `passing`: acceptance criteria passed; `evidence` must record proof

Use an execution plan under `.agents/exec_plans/` for cross-cutting,
safety-relevant, build-system, or target-deployment changes.

## External Inputs

Maintainers must provide and verify:

- AWS/ECR credentials able to pull the selected development and QNX builder
  images
- canonical AMD64 `QNX_BUILD_IMAGE`
- licensed QNX and RTI SDK contents baked into the published builder image
- a valid repository QNX toolchain and QNX license/configuration
- RTI archive contents containing both AMD64 Linux and QNX target libraries
- approved RTI and QNX MCP access required by repository policy
- QNX HAM development files (`<ha/ham.h>` and `libham`) in any builder expected
  to compile supervised QNX `sr_node` users
- QNX HAM runtime policy inputs for production nodes, including any
  `SR_HAM_RESTART_COMMAND`, heartbeat thresholds, and optional high
  missed-heartbeat escalation command
- QEMU or remote-target address, credentials, deployment path, and permission
  before runtime target testing

Never commit credentials, licenses, private keys, or licensed SDK artifacts.

## Verification Boundaries

`./init.sh verify` validates work tracking and docs, checks shell changes, and
runs the application verification script when it exists. Set `VERIFY_QNX=1` to
also require QNX cross-compilation.

The default gate does not deploy to or execute on QNX targets. QEMU, remote
target, hardware-in-the-loop, and vehicle testing must remain explicit,
target-scoped operations with recorded evidence.

For QNX HAM supervision changes, build success is not runtime acceptance.
Record a bounded target smoke test that demonstrates self-attach, heartbeat
delivery, low/high missed-heartbeat handling, process-death detection, restart,
normal detach on shutdown, and the exact HAM commands/environment used.

Dedicated bridge nodes that add MQTT or cloud dependencies need focused fake
publisher tests plus explicit target-library and runtime smoke evidence. That
scope is separate from ordinary `SRNode` verification.

## Suggested Workflow

1. A maintainer creates or prioritizes a work item.
2. An agent reads repository guidance and runs the baseline gate.
3. The agent creates an execution plan when required.
4. Implementation and focused verification proceed.
5. Evidence, docs, and work-item status are updated.
6. A maintainer authorizes any remote target or external mutation.
