import { Aws, CfnOutput, Duration, RemovalPolicy, Stack, StackProps } from 'aws-cdk-lib';
import * as codebuild from 'aws-cdk-lib/aws-codebuild';
import * as ecr from 'aws-cdk-lib/aws-ecr';
import * as iam from 'aws-cdk-lib/aws-iam';
import * as logs from 'aws-cdk-lib/aws-logs';
import * as s3 from 'aws-cdk-lib/aws-s3';
import { Construct } from 'constructs';

export interface DevEnvBuilderStackProps extends StackProps {
  readonly githubOwner: string;
  readonly githubRepo: string;
  readonly githubBranch: string;
  readonly projectName: string;
  readonly repositoryName: string;
  readonly imageTagPrefix: string;
  readonly qnxSdpKey: string;
  readonly qnxLicenseKey: string;
  readonly rtiKey: string;
  readonly tiPsdkRtosKey: string;
  readonly tiPsdkQnxKey: string;
  readonly tiQnxBspKey: string;
}

export class DevEnvBuilderStack extends Stack {
  constructor(scope: Construct, id: string, props: DevEnvBuilderStackProps) {
    super(scope, id, props);

    const inputBucket = new s3.Bucket(this, 'LicensedInputBucket', {
      blockPublicAccess: s3.BlockPublicAccess.BLOCK_ALL,
      encryption: s3.BucketEncryption.S3_MANAGED,
      enforceSSL: true,
      versioned: true,
      removalPolicy: RemovalPolicy.RETAIN,
      autoDeleteObjects: false,
      lifecycleRules: [
        {
          id: 'AbortIncompleteMultipartUploads',
          abortIncompleteMultipartUploadAfter: Duration.days(7)
        }
      ]
    });

    const imageRepository = new ecr.Repository(this, 'BuilderImageRepository', {
      repositoryName: props.repositoryName,
      imageTagMutability: ecr.TagMutability.MUTABLE,
      imageScanOnPush: true,
      encryption: ecr.RepositoryEncryption.AES_256,
      removalPolicy: RemovalPolicy.RETAIN
    });
    imageRepository.addLifecycleRule({
      description: 'Keep recent builder images',
      maxImageCount: 20,
      rulePriority: 1
    });

    const buildRole = new iam.Role(this, 'BuildRole', {
      assumedBy: new iam.ServicePrincipal('codebuild.amazonaws.com'),
      description: 'Allows CodeBuild to build and publish the Docker development environment image.'
    });
    inputBucket.grantRead(buildRole);
    imageRepository.grantPullPush(buildRole);
    buildRole.addToPolicy(new iam.PolicyStatement({
      actions: ['ecr:GetAuthorizationToken'],
      resources: ['*']
    }));

    const logGroup = new logs.LogGroup(this, 'BuildLogGroup', {
      retention: logs.RetentionDays.ONE_MONTH,
      removalPolicy: RemovalPolicy.DESTROY
    });

    const project = new codebuild.Project(this, 'DockerImageBuildProject', {
      projectName: props.projectName,
      description: 'Builds and publishes the generic Docker development environment image.',
      buildSpec: codebuild.BuildSpec.fromSourceFilename('docker/buildspec.yml'),
      source: codebuild.Source.gitHub({
        owner: props.githubOwner,
        repo: props.githubRepo,
        branchOrRef: props.githubBranch,
        cloneDepth: 1,
        webhook: false
      }),
      role: buildRole,
      timeout: Duration.hours(2),
      queuedTimeout: Duration.hours(4),
      logging: {
        cloudWatch: {
          enabled: true,
          logGroup
        }
      },
      environment: {
        buildImage: codebuild.LinuxBuildImage.STANDARD_7_0,
        privileged: true,
        computeType: codebuild.ComputeType.MEDIUM,
        environmentVariables: {
          AWS_DEFAULT_REGION: { value: Aws.REGION },
          AWS_ACCOUNT_ID: { value: Aws.ACCOUNT_ID },
          IMAGE_REPO_NAME: { value: imageRepository.repositoryName },
          ECR_URL: { value: `${Aws.ACCOUNT_ID}.dkr.ecr.${Aws.REGION}.${Aws.URL_SUFFIX}` },
          S3_BUCKET: { value: inputBucket.bucketName },
          QNX_SDP_KEY: { value: props.qnxSdpKey },
          QNX_LICENSE_KEY: { value: props.qnxLicenseKey },
          RTI_KEY: { value: props.rtiKey },
          TI_PSDK_RTOS_KEY: { value: props.tiPsdkRtosKey },
          TI_PSDK_QNX_KEY: { value: props.tiPsdkQnxKey },
          TI_QNX_BSP_KEY: { value: props.tiQnxBspKey },
          IMAGE_TAG_PREFIX: { value: props.imageTagPrefix }
        }
      }
    });

    new CfnOutput(this, 'InputBucketName', {
      value: inputBucket.bucketName,
      description: 'S3 bucket for licensed Docker build inputs.'
    });
    new CfnOutput(this, 'ImageRepositoryUri', {
      value: imageRepository.repositoryUri,
      description: 'ECR repository URI for the builder image.'
    });
    new CfnOutput(this, 'CodeBuildProjectName', {
      value: project.projectName,
      description: 'CodeBuild project that builds the Docker environment.'
    });
  }
}
