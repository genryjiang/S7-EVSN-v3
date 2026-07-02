#!/usr/bin/env node

import { readFile, writeFile } from 'node:fs/promises';
import { fileURLToPath } from 'node:url';
import { resolve } from 'node:path';

const repoRoot = resolve(fileURLToPath(new URL('..', import.meta.url)));
const input = resolve(repoRoot, 'out/qnx/compile_commands.container.json');
const output = resolve(repoRoot, 'out/qnx/compile_commands.json');

const replacements = [
  ['/home/developer/rti_connext_dds-7.3.1', resolve(repoRoot, '.intellisense/rti_connext_dds-7.3.1')],
  ['/home/developer/qnx800', resolve(repoRoot, '.intellisense/qnx800')],
  ['/workspace/project', repoRoot],
];

const editorFlags = [
  '-I', resolve(repoRoot, 'evsn-runtime/dds_types/std_types'),
  '-I', resolve(repoRoot, 'evsn-runtime/dds_types/custom_types'),
  '-I', resolve(repoRoot, 'evsn-runtime/evsn_node'),
  '-I', resolve(repoRoot, 'vcan-drivers/dev-can-linux'),
  '--target=aarch64-unknown-nto',
  '-isystem', resolve(repoRoot, '.intellisense/qnx800/target/qnx/usr/include/c++/v1'),
  '-isystem', resolve(repoRoot, '.intellisense/qnx800/host/linux/x86_64/usr/lib/gcc/aarch64-unknown-nto-qnx8.0.0/12.2.0/include'),
  '-isystem', resolve(repoRoot, '.intellisense/qnx800/target/qnx/usr/include'),
  '-isystem', resolve(repoRoot, '.intellisense/qnx800/target/qnx/usr/local/include'),
  '-isystem', resolve(repoRoot, '.intellisense/rti_connext_dds-7.3.1/include'),
  '-isystem', resolve(repoRoot, '.intellisense/rti_connext_dds-7.3.1/include/ndds'),
  '-isystem', resolve(repoRoot, '.intellisense/rti_connext_dds-7.3.1/include/ndds/hpp'),
  '-D__QNXNTO__=1',
  '-D__QNX__=800',
  '-D__LITTLEENDIAN__=1',
  '-DQNX_LIBM_BUILTINS=1',
];

const qccOnlyFlagPatterns = [
  /^-Y_cxx$/,
  /^-V.+/,
  /^-Wc,-isysroot,.+/,
  /^-lang-c\+\+$/,
];

function appendEditorFlags(command) {
  const flags = editorFlags.map((flag) => JSON.stringify(flag)).join(' ');
  return `${command} ${flags}`;
}

function rewriteCompilerForEditor(command) {
  return command
    .replace(/^\S*(?:q\+\+|qnx-aarch64-g\+\+)\s+/, 'clang++ ')
    .replace(/^\S*(?:qcc|qnx-aarch64-gcc)\s+/, 'clang ');
}

function stripQccOnlyFlags(command) {
  return command
    .replace(/\s-Y_cxx(?=\s|$)/g, '')
    .replace(/\s-V\S+/g, '')
    .replace(/\s-Wc,-isysroot,\S+/g, '')
    .replace(/\s-lang-c\+\+/g, '');
}

function isQccOnlyFlag(argument) {
  return qccOnlyFlagPatterns.some((pattern) => pattern.test(argument));
}

function rewriteArgumentsForEditor(argumentsList) {
  if (!Array.isArray(argumentsList) || argumentsList.length === 0) return undefined;

  const [compiler, ...args] = argumentsList;
  const compilerName = String(compiler).split('/').at(-1);
  const editorCompiler = compilerName?.includes('++') ? 'clang++' : 'clang';

  return [editorCompiler, ...args.filter((argument) => !isQccOnlyFlag(argument)), ...editorFlags];
}

function rewrite(value) {
  if (typeof value === 'string') {
    return replacements.reduce(
      (result, [containerPath, hostPath]) => result.replaceAll(containerPath, hostPath),
      value,
    );
  }
  if (Array.isArray(value)) return value.map(rewrite);
  if (value && typeof value === 'object') {
    return Object.fromEntries(Object.entries(value).map(([key, item]) => [key, rewrite(item)]));
  }
  return value;
}

const database = JSON.parse(await readFile(input, 'utf8'));
const hostDatabase = rewrite(database).map((entry) => {
  const command = entry.command
    ? stripQccOnlyFlags(rewriteCompilerForEditor(entry.command))
    : undefined;
  const argumentsList = entry.arguments
    ? rewriteArgumentsForEditor(entry.arguments)
    : undefined;

  return {
    ...entry,
    directory: repoRoot,
    ...(command ? { command: appendEditorFlags(command) } : {}),
    ...(argumentsList && !command ? { command: argumentsList.map((argument) => JSON.stringify(argument)).join(' ') } : {}),
    arguments: undefined,
  };
});

await writeFile(output, `${JSON.stringify(hostDatabase, null, 2)}\n`);
const outputText = await readFile(output, 'utf8');
const forbiddenPatterns = [
  /\s-Y_cxx(?=\s|")/,
  /qnx-aarch64-g\+\+/,
  /qnx-aarch64-gcc/,
  /\sq\+\+(?=\s|")/,
  /\sqcc(?=\s|")/,
];

for (const pattern of forbiddenPatterns) {
  if (pattern.test(outputText)) {
    throw new Error(`Generated clangd compile database still contains QNX-only compiler syntax: ${pattern}`);
  }
}

console.log(`Updated ${output}`);
