import test from 'brittle'
import { join } from 'path'
import tmp from 'test-tmp'
import { open, close, stat, read, write } from './helpers.mjs'

import { trim, sparse } from '../index.js'

test('explicit hole', async (t) => {
  const fd = await open(join(await tmp(t), 'test'), 'w+')
  t.teardown(() => close(fd))

  await sparse(fd)

  const { blksize } = await stat(fd)

  const empty = Buffer.alloc(blksize * 1000)
  await write(fd, empty)

  const expected = Buffer.from('hello world')
  await write(fd, expected)

  await testTrim(t, fd, 0, empty.byteLength)

  const actual = Buffer.alloc(expected.byteLength)
  await read(fd, actual, 0, expected.byteLength, empty.byteLength)

  t.alike(actual, expected, 'file is intact')
})

test('implicit hole', async (t) => {
  const fd = await open(join(await tmp(t), 'test'), 'w+')
  t.teardown(() => close(fd))

  await sparse(fd)

  const { blksize } = await stat(fd)

  const empty = blksize * 1000

  const hello = Buffer.from('hello world')
  await write(fd, hello, 0, hello.byteLength, empty)

  const { blocks } = await stat(fd)

  t.comment(`${blocks} blocks`)
  t.pass()
})

test('unaligned hole', async (t) => {
  const fd = await open(join(await tmp(t), 'test'), 'w+')
  t.teardown(() => close(fd))

  await sparse(fd)

  const { blksize } = await stat(fd)

  const empty = Buffer.alloc(blksize * 1000)
  await write(fd, empty)

  await testTrim(t, fd, blksize / 2, empty.byteLength)
})

async function testTrim(t, fd, offset, length) {
  await testReducesBlocks(t, fd, () => trim(fd, offset, length))
}

async function testReducesBlocks(t, fd, fn) {
  const { blocks: before } = await stat(fd)

  await fn()

  const { blocks: after } = await stat(fd)

  t.comment(`${before} -> ${after} blocks`)
  t.ok(after < before, 'blocks reduced')
}
