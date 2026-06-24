import { readFile } from 'node:fs/promises';

const workItemsPath = new URL('./feature_list.json', import.meta.url);
const data = JSON.parse(await readFile(workItemsPath, 'utf8'));
const errors = [];
const statuses = new Set(['not_started', 'in_progress', 'blocked', 'passing']);
const idPattern = /^VSN-\d{4}$/;

if (data.project !== 'S7-EVSN') {
  errors.push('project must be "S7-EVSN"');
}

if (!/^\d{4}-\d{2}-\d{2}$/.test(data.last_updated ?? '')) {
  errors.push('last_updated must use YYYY-MM-DD');
}

if (!Array.isArray(data.items) || data.items.length === 0) {
  errors.push('items must be a non-empty array');
} else {
  const ids = new Set();

  for (const [index, item] of data.items.entries()) {
    const location = `items[${index}]`;
    if (!item.id || ids.has(item.id)) errors.push(`${location}.id must be unique`);
    if (!idPattern.test(item.id ?? '')) errors.push(`${location}.id must use VSN-XXXX format`);
    ids.add(item.id);
    if (!Number.isInteger(item.priority)) errors.push(`${location}.priority must be an integer`);
    if (!statuses.has(item.status)) errors.push(`${location}.status is invalid`);
    for (const field of ['acceptance_criteria', 'verification_commands', 'evidence', 'dependencies', 'blockers']) {
      if (!Array.isArray(item[field])) errors.push(`${location}.${field} must be an array`);
    }
    if (item.status === 'passing' && item.evidence.length === 0) {
      errors.push(`${location}.evidence is required when status is passing`);
    }
    if (item.status === 'blocked' && item.blockers.length === 0) {
      errors.push(`${location}.blockers is required when status is blocked`);
    }
  }

  for (const [index, item] of data.items.entries()) {
    const location = `items[${index}]`;
    for (const dependency of item.dependencies) {
      if (!idPattern.test(dependency)) {
        errors.push(`${location}.dependencies contains non-VSN ID: ${dependency}`);
      } else if (!ids.has(dependency)) {
        errors.push(`${location}.dependencies references unknown work item: ${dependency}`);
      }
    }
  }
}

if (errors.length > 0) {
  console.error('Invalid .agents/feature_list.json:');
  for (const error of errors) console.error(`- ${error}`);
  process.exit(1);
}

console.log(`Validated ${data.items.length} work item(s)`);
