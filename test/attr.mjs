import test from 'brittle'
import { join } from 'path'
import tmp from 'test-tmp'
import { open, close } from './helpers.mjs'

import { getAttr, setAttr } from '../index.js'

test('set and get attribute', async (t) => {
  const file = join(await tmp(t), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  await setAttr(fd, 'user.hello', 'hello world')

  t.alike(await getAttr(fd, 'user.hello'), Buffer.from('hello world'))
})
