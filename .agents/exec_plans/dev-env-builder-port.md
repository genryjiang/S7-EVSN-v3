# dev-env-builder-port

Branch: current branch
Owner: main-agent
Status: complete

## Scope

- Port the Docker builder environment into a generic root `docker/` folder.
- Add a separate top-level `Dev-Env-Builder/` CDK app.
- Keep the AWS stack focused on building and publishing the Docker environment.
- Remove project-specific names from the new Docker and AWS infrastructure
  files.

## Non-Goals

- Do not provision devboxes, CodePipeline, Lambda APIs, CodeArtifact, GitHub
  OIDC roles, or organization-specific IAM groups.
- Do not commit licensed QNX, RTI, or license archives.
- Do not run an actual Docker image build or AWS deployment.

## Context Loaded

- Source Docker runtime: `/Users/henryjiang/Documents/Sunswift/SR-Mjolnir/docker/`
- Source builder image and buildspec:
  `/Users/henryjiang/Documents/Sunswift/EMBD-High-Dev-Infra/docker/`
- Source stack:
  `/Users/henryjiang/Documents/Sunswift/EMBD-High-Dev-Infra/lib/srp-130_aws_infra-stack.ts`
- AWS CDK best practices from AWS MCP

## Steps

- [x] Add generic `docker/` Compose, env template, buildspec, Dockerfile, and README.
- [x] Add `Dev-Env-Builder/` CDK app for S3, ECR, IAM role, CloudWatch Logs, and one CodeBuild project.
- [x] Add CDK tests proving only the builder infrastructure surface is synthesized.
- [x] Scrub new Docker/AWS files for project-specific names.
- [x] Validate synthesized CloudFormation through AWS MCP.

## Verification

- [x] `rg -n "Sunswift|UNSW|SRP|SRPAWS|SR-Mjolnir|sr-mjolnir|Srp130|srp-130|EMBD|Asgard|apsis" docker Dev-Env-Builder`
      Result: no matches.
- [x] `npm run build`
      Result: passed.
- [x] `npm test -- --runInBand`
      Result: passed, 3 tests.
- [x] `npm run synth`
      Result: passed.
- [x] AWS MCP `validate_cloudformation_template`
      Result: valid, 0 errors, 0 warnings, 0 info messages.

## Evidence

- CDK synth output: `Dev-Env-Builder/cdk.out/DevEnvBuilderStack.template.json`
- CDK tests: `Dev-Env-Builder/test/dev-env-builder.test.ts`

## Handoff

Set real GitHub source context in `Dev-Env-Builder/cdk.json` or via `-c`
before deployment. Upload licensed inputs to the generated S3 bucket before
starting CodeBuild.
