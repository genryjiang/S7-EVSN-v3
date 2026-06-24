import * as cdk from 'aws-cdk-lib';
import { Match, Template } from 'aws-cdk-lib/assertions';
import { DevEnvBuilderStack } from '../lib/dev-env-builder-stack';

function synthTemplate() {
  const app = new cdk.App();
  const stack = new DevEnvBuilderStack(app, 'TestStack', {
    githubOwner: 'example-owner',
    githubRepo: 'example-repo',
    githubBranch: 'main',
    projectName: 'Dev-Env-Builder',
    repositoryName: 'dev-env-builder',
    imageTagPrefix: 'dev-env-builder-amd64',
    qnxSdpKey: 'qnx/qnx-sdp.tar.gz',
    qnxLicenseKey: 'qnx/qnx-license.tar.gz',
    rtiKey: 'rti/rti_connext-7.3.1.tar.gz'
  });
  return Template.fromStack(stack);
}

test('provisions only the Docker builder infrastructure surface', () => {
  const template = synthTemplate();

  template.resourceCountIs('AWS::CodeBuild::Project', 1);
  template.resourceCountIs('AWS::ECR::Repository', 1);
  template.resourceCountIs('AWS::S3::Bucket', 1);
  template.resourceCountIs('AWS::CodePipeline::Pipeline', 0);
  template.resourceCountIs('AWS::CodeStarConnections::Connection', 0);
  template.resourceCountIs('AWS::CodeArtifact::Repository', 0);
  template.resourceCountIs('AWS::Lambda::Function', 0);
  template.resourceCountIs('AWS::EC2::Instance', 0);
});

test('CodeBuild builds the root Docker environment with privileged Docker', () => {
  const template = synthTemplate();

  template.hasResourceProperties('AWS::CodeBuild::Project', {
    Name: 'Dev-Env-Builder',
    Artifacts: { Type: 'NO_ARTIFACTS' },
    Source: Match.objectLike({
      Type: 'GITHUB',
      BuildSpec: 'docker/buildspec.yml',
      Location: 'https://github.com/example-owner/example-repo.git'
    }),
    Environment: Match.objectLike({
      Type: 'LINUX_CONTAINER',
      ComputeType: 'BUILD_GENERAL1_MEDIUM',
      Image: 'aws/codebuild/standard:7.0',
      PrivilegedMode: true,
      EnvironmentVariables: Match.arrayWith([
        Match.objectLike({ Name: 'IMAGE_REPO_NAME', Value: Match.anyValue() }),
        Match.objectLike({ Name: 'QNX_SDP_KEY', Value: 'qnx/qnx-sdp.tar.gz' }),
        Match.objectLike({ Name: 'QNX_LICENSE_KEY', Value: 'qnx/qnx-license.tar.gz' }),
        Match.objectLike({ Name: 'RTI_KEY', Value: 'rti/rti_connext-7.3.1.tar.gz' }),
        Match.objectLike({ Name: 'IMAGE_TAG_PREFIX', Value: 'dev-env-builder-amd64' })
      ])
    })
  });
});

test('stateful storage has explicit retention and secure defaults', () => {
  const template = synthTemplate();

  template.hasResourceProperties('AWS::S3::Bucket', {
    BucketEncryption: {
      ServerSideEncryptionConfiguration: [
        {
          ServerSideEncryptionByDefault: {
            SSEAlgorithm: 'AES256'
          }
        }
      ]
    },
    PublicAccessBlockConfiguration: {
      BlockPublicAcls: true,
      BlockPublicPolicy: true,
      IgnorePublicAcls: true,
      RestrictPublicBuckets: true
    },
    VersioningConfiguration: {
      Status: 'Enabled'
    }
  });

  template.hasResourceProperties('AWS::ECR::Repository', {
    RepositoryName: 'dev-env-builder',
    ImageScanningConfiguration: {
      ScanOnPush: true
    },
    ImageTagMutability: 'MUTABLE'
  });
});
