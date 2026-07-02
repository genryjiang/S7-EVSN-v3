# Docker Environment

This folder contains a generic licensed QNX/RTI builder environment.

- `Dockerfile`: builds the AMD64 builder image.
- `buildspec.yml`: CodeBuild entrypoint for building and publishing the image.
- `compose.yml`: local Compose service that runs the published builder image.
- `.env.example`: local Compose configuration template.

The image build expects licensed archives to be available from the S3 bucket
provisioned by `Dev-Env-Builder`:

| Key | Expected contents |
| --- | --- |
| `qnx/qnx-sdp.tar.gz` | QNX SDP host and target files |
| `qnx/qnx-license.tar.gz` | QNX license/configuration files |
| `rti/rti_connext-7.3.1.tar.gz` | RTI Connext files, including Linux and QNX libraries |
| `ti/ti-processor-sdk-rtos-j784s4-evm-11_02_00_06.tar.gz` | TI Processor SDK RTOS J784S4 install tree |
| `ti/ti-processor-sdk-qnx_800_j784s4_11_02_00_04.tar.gz` | TI Processor SDK QNX package extracted under the RTOS SDK |
| `ti/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342.zip` | TI J784S4 QNX BSP archive expected by `psdk_qnx_setup.sh` |

Do not commit licensed SDK archives, license files, credentials, or exported SDK
headers.
