const test = require('brittle')
const { join, sep } = require('path')
const { writeFile, readFile } = require('fs/promises')
const { isWindows } = require('which-runtime')

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

test('swap files, long paths', { skip: isWindows }, async (t) => {
  const padding = `.${sep}`.repeat(200)

  const a = (await t.tmp()) + sep + padding + 'a'
  const b = (await t.tmp()) + sep + padding + 'b'

  await writeFile(a, 'a')
  await writeFile(b, 'b')

  await swap(a, b)
  swapSync(a, b)
  await swap(a, b)

  t.is(await readFile(a, 'utf8'), 'b')
  t.is(await readFile(b, 'utf8'), 'a')
})

test('swap files, paths longer than the platform allows', async (t) => {
  const long = 'a'.repeat(5000)

  await t.exception(swap(long, long))
  t.exception(() => swapSync(long, long))
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
