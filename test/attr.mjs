import test from 'brittle'
import { join } from 'path'
import tmp from 'test-tmp'
import { open, close } from './helpers.mjs'

import { getAttr, setAttr, listAttrs } from '../index.js'

test('set and get attribute', async (t) => {
  const file = join(await tmp(t), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.hello', 'hello world')

  t.alike(await getAttr(fd, 'user.hello'), Buffer.from('hello world'))
})

test('set and list attributes', async (t) => {
  const file = join(await tmp(t), 'test')

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
