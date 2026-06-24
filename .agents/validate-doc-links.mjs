import { existsSync } from 'node:fs';
import { readdir, readFile } from 'node:fs/promises';
import { dirname, join, resolve } from 'node:path';

const ignoredDirectories = new Set(['.git', 'build', 'deploy', 'node_modules', 'out']);
const files = [];
const errors = [];

async function collectMarkdownFiles(directory) {
  for (const entry of await readdir(directory, { withFileTypes: true })) {
    if (entry.isDirectory() && ignoredDirectories.has(entry.name)) continue;

    const path = join(directory, entry.name);
    if (entry.isDirectory()) await collectMarkdownFiles(path);
    if (entry.isFile() && entry.name.endsWith('.md')) files.push(path);
  }
}

await collectMarkdownFiles('.');

for (const file of files) {
  const text = await readFile(file, 'utf8');
  const links = text.matchAll(/\[[^\]]+\]\(([^)#]+)(?:#[^)]+)?\)/g);

  for (const match of links) {
    const target = match[1];
    if (/^[a-z]+:/i.test(target)) continue;

    const path = resolve(dirname(file), target);
    if (!existsSync(path)) errors.push(`${file}: missing ${target}`);
  }
}

if (errors.length > 0) {
  console.error('Invalid documentation links:');
  for (const error of errors) console.error(`- ${error}`);
  process.exit(1);
}

console.log(`Validated links in ${files.length} Markdown file(s)`);
