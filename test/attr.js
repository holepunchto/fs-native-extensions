const test = require('brittle')
const { join } = require('path')
const { isWindows } = require('which-runtime')
const { open, close } = require('./helpers')

const {
  getAttr,
  setAttr,
  removeAttr,
  listAttrs,
  getAttrSync,
  setAttrSync,
  removeAttrSync,
  listAttrsSync
} = require('..')

test('set and get attribute', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.hello', 'hello world')

  t.alike(await getAttr(fd, 'user.hello'), Buffer.from('hello world'))
})

test('get missing attribute', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  t.is(await getAttr(fd, 'user.hello'), null)
})

test('set and remove attribute', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.hello', 'hello world')

  await removeAttr(fd, 'user.hello')

  t.is(await getAttr(fd, 'user.hello'), null)
})

test('remove missing attribute', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await removeAttr(fd, 'user.hello')
})

test('set and list attributes', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.a', 'hello a')
  await setAttr(fd, 'user.b', 'hello b')
  await setAttr(fd, 'user.c', 'hello c')
  await setAttr(fd, 'user.d', 'hello d')

  const attrs = await listAttrs(fd)

  t.alike(
    attrs.filter((a) => a.startsWith('user.')),
    ['user.a', 'user.b', 'user.c', 'user.d']
  )
})

test('set and list attribute with non-ascii name', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.\u00e6\u00f8\u00e5', 'hello world')

  const attrs = await listAttrs(fd)

  t.alike(
    attrs.filter((a) => a.startsWith('user.')),
    ['user.\u00e6\u00f8\u00e5']
  )
  t.alike(await getAttr(fd, 'user.\u00e6\u00f8\u00e5'), Buffer.from('hello world'))
})

test('set attribute with long name', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await t.exception(setAttr(fd, 'user.' + 'a'.repeat(1000), 'hello world'))
})

test('get attribute while stream is open for writing', { skip: !isWindows }, async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.hello', 'hello world')

  const stream = await open(file + ':user.hello', 'r+')
  t.teardown(() => close(stream))

  t.alike(await getAttr(fd, 'user.hello'), Buffer.from('hello world'))
})

test('set and get attribute, sync', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  setAttrSync(fd, 'user.hello', 'hello world')

  t.alike(getAttrSync(fd, 'user.hello'), Buffer.from('hello world'))
})

test('get missing attribute, sync', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  t.is(getAttrSync(fd, 'user.hello'), null)
})

test('set and remove attribute, sync', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  setAttrSync(fd, 'user.hello', 'hello world')

  removeAttrSync(fd, 'user.hello')

  t.is(getAttrSync(fd, 'user.hello'), null)
})

test('remove missing attribute, sync', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  removeAttrSync(fd, 'user.hello')
})

test('set and list attributes, sync', async (t) => {
  const file = join(await t.tmp(), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  setAttrSync(fd, 'user.a', 'hello a')
  setAttrSync(fd, 'user.b', 'hello b')
  setAttrSync(fd, 'user.c', 'hello c')
  setAttrSync(fd, 'user.d', 'hello d')

  const attrs = listAttrsSync(fd)

  t.alike(
    attrs.filter((a) => a.startsWith('user.')),
    ['user.a', 'user.b', 'user.c', 'user.d']
  )
})
