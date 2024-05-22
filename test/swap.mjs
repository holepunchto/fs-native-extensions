import test from 'brittle'
import { join } from 'path'
import { writeFile, readFile } from 'fs/promises'
import tmp from 'test-tmp'
import { isWindows } from 'which-runtime'

import { swap } from '../index.js'

// TODO: Swapping is borked on Windows currently, investigate.

test('swap files', { skip: isWindows }, async (t) => {
  const a = join(await tmp(t), 'a')
  const b = join(await tmp(t), 'b')

  await writeFile(a, 'a')
  await writeFile(b, 'b')

  await swap(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(b, 'utf8'), 'a')
})

test('swap directories', { skip: isWindows }, async (t) => {
  const a = await tmp(t)
  const b = await tmp(t)

  await writeFile(join(a, 'a'), 'a')
  await writeFile(join(b, 'b'), 'b')

  await swap(a, b)

  t.is(await readFile(join(a, 'b'), 'utf8'), 'b')
  t.is(await readFile(join(b, 'a'), 'utf8'), 'a')
})

test('swap file and directory', { skip: isWindows }, async (t) => {
  const a = await tmp(t)
  const b = join(await tmp(t), 'b')

  await writeFile(join(a, 'a'), 'a')
  await writeFile(b, 'b')

  await swap(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(join(b, 'a'), 'utf8'), 'a')
})
