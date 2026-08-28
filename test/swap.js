const test = require('brittle')
const { join } = require('path')
const { writeFile, readFile } = require('fs/promises')

const { swap, swapSync } = require('..')

test('swap files', async (t) => {
  const a = join(await t.tmp(), 'a')
  const b = join(await t.tmp(), 'b')

  await writeFile(a, 'a')
  await writeFile(b, 'b')

  await swap(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(b, 'utf8'), 'a')
})

test('swap directories', async (t) => {
  const a = await t.tmp()
  const b = await t.tmp()

  await writeFile(join(a, 'a'), 'a')
  await writeFile(join(b, 'b'), 'b')

  await swap(a, b)

  t.is(await readFile(join(a, 'b'), 'utf8'), 'b')
  t.is(await readFile(join(b, 'a'), 'utf8'), 'a')
})

test('swap file and directory', async (t) => {
  const a = await t.tmp()
  const b = join(await t.tmp(), 'b')

  await writeFile(join(a, 'a'), 'a')
  await writeFile(b, 'b')

  await swap(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(join(b, 'a'), 'utf8'), 'a')
})

test('swap files, sync', async (t) => {
  const a = join(await t.tmp(), 'a')
  const b = join(await t.tmp(), 'b')

  await writeFile(a, 'a')
  await writeFile(b, 'b')

  swapSync(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(b, 'utf8'), 'a')
})

test('swap directories, sync', async (t) => {
  const a = await t.tmp()
  const b = await t.tmp()

  await writeFile(join(a, 'a'), 'a')
  await writeFile(join(b, 'b'), 'b')

  swapSync(a, b)

  t.is(await readFile(join(a, 'b'), 'utf8'), 'b')
  t.is(await readFile(join(b, 'a'), 'utf8'), 'a')
})
