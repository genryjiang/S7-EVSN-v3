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
  readonly tiPsdkRtosKey?: string;
  readonly tiPsdkQnxKey?: string;
  readonly tiQnxBspKey?: string;
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
  rtiKey: context.rtiKey ?? 'rti/rti_connext-7.3.1.tar.gz',
  tiPsdkRtosKey: context.tiPsdkRtosKey ?? 'ti/ti-processor-sdk-rtos-j784s4-evm-11_02_00_06.tar.gz',
  tiPsdkQnxKey: context.tiPsdkQnxKey ?? 'ti/ti-processor-sdk-qnx_800_j784s4_11_02_00_04.tar.gz',
  tiQnxBspKey: context.tiQnxBspKey ?? 'ti/BSP_ti-j784s4-evm_be-800_SVN1019295_JBN342.zip'
});
