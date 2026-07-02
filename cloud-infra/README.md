# Dev-Env-Builder

This CDK app provisions the AWS resources required to build and publish the
generic Docker development environment image.

It creates:

- one S3 bucket for licensed build inputs
- one ECR repository for the builder image
- one IAM role for CodeBuild
- one privileged AMD64 CodeBuild project named `Dev-Env-Builder`

It does not create CodePipeline, devboxes, Lambda APIs, CodeArtifact resources,
GitHub OIDC roles, or organization-specific IAM groups.

## Configure

Update `cdk.json` or pass context values at synth/deploy time:

```bash
npx cdk synth \
  -c devEnvBuilder.githubOwner=example-owner \
  -c devEnvBuilder.githubRepo=example-repo \
  -c devEnvBuilder.githubBranch=main
```

The CodeBuild project expects the source repository to contain `docker/` at the
repository root.

## Licensed Inputs

Upload these archives to the generated S3 bucket before starting a build:

| Context key | Default S3 key | Expected contents |
| --- | --- | --- |
| `qnxSdpKey` | `qnx/qnx-sdp.tar.gz` | QNX SDP host and target files |
| `qnxLicenseKey` | `qnx/qnx-license.tar.gz` | QNX license/configuration files |
| `rtiKey` | `rti/rti_connext-7.3.1.tar.gz` | RTI Connext files |
| `tiPsdkRtosKey` | `ti/ti-processor-sdk-rtos-j784s4-evm-11_02_00_06.tar.gz` | TI Processor SDK RTOS J784S4 install tree |
| `tiPsdkQnxKey` | `ti/ti-processor-sdk-qnx_800_j784s4_11_02_00_04.tar.gz` | TI Processor SDK QNX package extracted under the RTOS SDK |
| `tiQnxBspKey` | `ti/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342.zip` | TI J784S4 QNX BSP archive expected by `psdk_qnx_setup.sh` |

Do not commit licensed SDK archives, license files, credentials, or exported SDK
headers.

## Commands

```bash
npm install
npm run build
npm test
npm run synth
```
