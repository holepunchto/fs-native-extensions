import test from 'brittle'
import { join } from 'path'
import tmp from 'test-tmp'
import { isWindows, isBare } from 'which-runtime'
import { open, close } from './helpers.mjs'

import { tryLock, tryDowngradeLock, tryUpgradeLock, unlock } from '../index.js'

test('2 exclusive locks, same fd', async (t) => {
  const file = join(await tmp(t), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  t.ok(tryLock(fd), 'lock granted')

  if (isWindows) {
    t.absent(tryLock(fd), 'lock denied')
  } else {
    t.ok(tryLock(fd), 'lock granted')
  }
})

test('2 exclusive locks, separate fd', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')
  t.teardown(() => close(a))

  const b = await open(file, 'w+')
  t.teardown(() => close(b))

  t.ok(tryLock(a), 'lock granted')
  t.absent(tryLock(b), 'lock denied')
})

test('2 shared locks + 1 exclusive lock, same fd', async (t) => {
  const file = join(await tmp(t), 'test')

  const fd = await open(file, 'w+')
  t.teardown(() => close(fd))

  t.ok(tryLock(fd, { shared: true }), 'lock granted')
  t.ok(tryLock(fd, { shared: true }), 'lock granted')

  if (isWindows) {
    t.absent(tryLock(fd), 'lock denied')
  } else {
    t.ok(tryLock(fd), 'lock granted')
  }
})

test('2 shared locks + 1 exclusive lock, separate fd', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')
  t.teardown(() => close(a))

  const b = await open(file, 'w+')
  t.teardown(() => close(b))

  const c = await open(file, 'w+')
  t.teardown(() => close(c))

  t.ok(tryLock(a, { shared: true }), 'lock granted')
  t.ok(tryLock(b, { shared: true }), 'lock granted')

  t.absent(tryLock(c), 'lock denied')

  unlock(a)
  unlock(b)

  t.ok(tryLock(c), 'lock granted')
})

test('2 shared locks + 1 exclusive lock, separate process', { skip: isBare }, async (t) => {
  const { fork } = await import('child_process')

  const shared = t.test('grant shared locks')
  shared.plan(2)

  const file = join(await tmp(t), 'test')

  const fd = await open(file, 'w+')

  const p1 = fork('test/fixture/lock.mjs', [file,
    '--mode', 'r',
    '--shared'
  ])
  const p2 = fork('test/fixture/lock.mjs', [file,
    '--mode', 'r',
    '--shared'
  ])

  p1.on('message', (message) => {
    shared.alike(message, { granted: true }, 'lock granted')
  })

  p2.on('message', (message) => {
    shared.alike(message, { granted: true }, 'lock granted')
  })

  await shared

  const deny = t.test('deny exclusive lock')
  deny.plan(1)

  const p3 = fork('test/fixture/lock.mjs', [file])

  p3.once('message', (message) => {
    deny.alike(message, { granted: false }, 'lock denied')
  })

  await deny

  const release = t.test('release shared locks')
  release.plan(2)

  p1.on('close', () => release.pass('lock released')).kill()
  p2.on('close', () => release.pass('lock released')).kill()

  await release

  const grant = t.test('grant exclusive lock')
  grant.plan(1)

  p3.on('message', (message) => {
    grant.alike(message, { granted: true }, 'lock granted')
  })

  await grant

  p3.kill()

  await close(fd)
})

test('lock is released on file close', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')

  const b = await open(file, 'w+')
  t.teardown(() => close(b))

  t.ok(tryLock(a), 'lock granted')

  t.absent(tryLock(b), 'lock denied')

  await t.execution(close(a), 'file closed, lock released')

  t.ok(tryLock(b), 'lock granted')
})

test('lock is not released on unrelated file close', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')
  t.teardown(() => close(a))

  const b = await open(file, 'w+')

  const c = await open(file, 'w+')
  t.teardown(() => close(c))

  t.ok(tryLock(a), 'lock granted')

  await t.execution(close(b), 'file closed, lock intact')

  t.absent(tryLock(c), 'lock denied')
})

test('downgrade exclusive lock', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')
  t.teardown(() => close(a))

  const b = await open(file, 'w+')
  t.teardown(() => close(b))

  t.ok(tryLock(a), 'lock granted')

  t.absent(tryLock(b, { shared: true }), 'lock denied')

  t.ok(tryDowngradeLock(a), 'lock downgraded')

  t.ok(tryLock(b, { shared: true }), 'lock granted')
})

test('upgrade shared lock', async (t) => {
  const file = join(await tmp(t), 'test')

  const a = await open(file, 'w+')
  t.teardown(() => close(a))

  const b = await open(file, 'w+')
  t.teardown(() => close(b))

  t.ok(tryLock(a, { shared: true }), 'lock granted')

  t.ok(tryLock(b, { shared: true }), 'lock granted')
  unlock(b)

  t.ok(tryUpgradeLock(a), 'lock upgraded')

  t.absent(tryLock(b, { shared: true }), 'lock denied')
})
