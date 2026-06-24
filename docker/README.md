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

Do not commit licensed SDK archives, license files, credentials, or exported SDK
headers.
