# 2026-06-20 - dev-env-builder-port

## Actions

- Ported the local Compose runtime into `docker/compose.yml`.
- Added a generic `docker/Dockerfile`, `docker/buildspec.yml`, and
  `docker/.env.example`.
- Added a separate `Dev-Env-Builder/` CDK app that provisions S3 input storage,
  ECR image storage, a CodeBuild IAM role, a retained log group, and one
  privileged AMD64 CodeBuild project.
- Removed project-specific names from the new Docker and AWS infrastructure
  files.

## Verification

- New Docker/AWS files scrub check returned no matches for project-specific
  source names.
- `npm run build` passed in `Dev-Env-Builder/`.
- `npm test -- --runInBand` passed 3 tests in `Dev-Env-Builder/`.
- `npm run synth` passed in `Dev-Env-Builder/`.
- AWS MCP `validate_cloudformation_template` reported the synthesized template
  valid with 0 errors, 0 warnings, and 0 info messages.
