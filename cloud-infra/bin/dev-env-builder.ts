#!/usr/bin/env node
import * as cdk from 'aws-cdk-lib';
import { DevEnvBuilderStack } from '../lib/dev-env-builder-stack';

interface DevEnvBuilderContext {
  readonly githubOwner?: string;
  readonly githubRepo?: string;
  readonly githubBranch?: string;
  readonly projectName?: string;
  readonly repositoryName?: string;
  readonly imageTagPrefix?: string;
  readonly qnxSdpKey?: string;
  readonly qnxLicenseKey?: string;
  readonly rtiKey?: string;
}

const app = new cdk.App();
const context = (app.node.tryGetContext('devEnvBuilder') ?? {}) as DevEnvBuilderContext;

new DevEnvBuilderStack(app, 'DevEnvBuilderStack', {
  githubOwner: context.githubOwner ?? 'example-owner',
  githubRepo: context.githubRepo ?? 'example-repo',
  githubBranch: context.githubBranch ?? 'main',
  projectName: context.projectName ?? 'Dev-Env-Builder',
  repositoryName: context.repositoryName ?? 'dev-env-builder',
  imageTagPrefix: context.imageTagPrefix ?? 'dev-env-builder-amd64',
  qnxSdpKey: context.qnxSdpKey ?? 'qnx/qnx-sdp.tar.gz',
  qnxLicenseKey: context.qnxLicenseKey ?? 'qnx/qnx-license.tar.gz',
  rtiKey: context.rtiKey ?? 'rti/rti_connext-7.3.1.tar.gz'
});
