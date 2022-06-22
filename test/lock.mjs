import test from 'brittle'
import { fork } from 'child_process'
import { open } from 'fs/promises'
import { temporaryFile } from 'tempy'
import { tryLock, tryDowngradeLock, tryUpgradeLock, unlock } from '../index.js'

const isWindows = process.platform === 'win32'

test('2 exclusive locks, same fd', async (t) => {
  const file = temporaryFile()

  const handle = await open(file, 'w+')
  t.teardown(() => handle.close())

  t.ok(tryLock(handle.fd), 'lock granted')

  if (isWindows) {
    t.absent(tryLock(handle.fd), 'lock denied')
  } else {
    t.ok(tryLock(handle.fd), 'lock granted')
  }
})

test('2 exclusive locks, separate fd', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')
  t.teardown(() => a.close())

  const b = await open(file, 'w+')
  t.teardown(() => b.close())

  t.ok(tryLock(a.fd), 'lock granted')
  t.absent(tryLock(b.fd), 'lock denied')
})

test('2 shared locks + 1 exclusive lock, same fd', async (t) => {
  const file = temporaryFile()

  const handle = await open(file, 'w+')
  t.teardown(() => handle.close())

  t.ok(tryLock(handle.fd, { shared: true }), 'lock granted')
  t.ok(tryLock(handle.fd, { shared: true }), 'lock granted')

  if (isWindows) {
    t.absent(tryLock(handle.fd), 'lock denied')
  } else {
    t.ok(tryLock(handle.fd), 'lock granted')
  }
})

test('2 shared locks + 1 exclusive lock, separate fd', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')
  t.teardown(() => a.close())

  const b = await open(file, 'w+')
  t.teardown(() => b.close())

  const c = await open(file, 'w+')
  t.teardown(() => c.close())

  t.ok(tryLock(a.fd, { shared: true }), 'lock granted')
  t.ok(tryLock(b.fd, { shared: true }), 'lock granted')

  t.absent(tryLock(c.fd), 'lock denied')

  unlock(a.fd)
  unlock(b.fd)

  t.ok(tryLock(c.fd), 'lock granted')
})

test('2 shared locks + 1 exclusive lock, separate process', async (t) => {
  const shared = t.test('grant shared locks')
  shared.plan(2)

  const file = temporaryFile()
  const handle = await open(file, 'w+')

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

  await handle.close()
})

test('lock is released on file close', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')

  const b = await open(file, 'w+')
  t.teardown(() => b.close())

  t.ok(tryLock(a.fd), 'lock granted')

  t.absent(tryLock(b.fd), 'lock denied')

  await t.execution(a.close(), 'file closed, lock released')

  t.ok(tryLock(b.fd), 'lock granted')
})

test('lock is not released on unrelated file close', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')
  t.teardown(() => a.close())

  const b = await open(file, 'w+')

  const c = await open(file, 'w+')
  t.teardown(() => c.close())

  t.ok(tryLock(a.fd), 'lock granted')

  await t.execution(b.close(), 'file closed, lock intact')

  t.absent(tryLock(c.fd), 'lock denied')
})

test('downgrade exclusive lock', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')
  t.teardown(() => a.close())

  const b = await open(file, 'w+')
  t.teardown(() => b.close())

  t.ok(tryLock(a.fd), 'lock granted')

  t.absent(tryLock(b.fd, { shared: true }), 'lock denied')

  t.ok(tryDowngradeLock(a.fd), 'lock downgraded')

  t.ok(tryLock(b.fd, { shared: true }), 'lock granted')
})

test('upgrade shared lock', async (t) => {
  const file = temporaryFile()

  const a = await open(file, 'w+')
  t.teardown(() => a.close())

  const b = await open(file, 'w+')
  t.teardown(() => b.close())

  t.ok(tryLock(a.fd, { shared: true }), 'lock granted')

  t.ok(tryLock(b.fd, { shared: true }), 'lock granted')
  unlock(b.fd)

  t.ok(tryUpgradeLock(a.fd), 'lock upgraded')

  t.absent(tryLock(b.fd, { shared: true }), 'lock denied')
})
