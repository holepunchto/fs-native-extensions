const errors = require('uv-errors')
const binding = require('./binding')

function onwork (errno) {
  if (errno < 0) this.reject(toError(errno))
  else this.resolve()
}

exports.tryLock = function tryLock (fd, offset = 0, length = 0, opts = {}) {
  if (typeof offset === 'object') {
    opts = offset
    offset = 0
  }

  if (typeof length === 'object') {
    opts = length
    length = 0
  }

  if (typeof opts !== 'object') {
    opts = {}
  }

  const errno = binding.fs_ext_napi_try_lock(fd, offset, length, opts.shared ? 0 : 1)

  if (errno < 0) {
    const err = toError(errno)
    if (err.code === 'EAGAIN') return false
    throw err
  }

  return true
}

exports.waitForLock = function waitForLock (fd, offset = 0, length = 0, opts = {}) {
  if (typeof offset === 'object') {
    opts = offset
    offset = 0
  }

  if (typeof length === 'object') {
    opts = length
    length = 0
  }

  if (typeof opts !== 'object') {
    opts = {}
  }

  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_lock_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_wait_for_lock(req, fd, offset, length, opts.shared ? 0 : 1, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

exports.tryDowngradeLock = function tryDowngradeLock (fd, offset = 0, length = 0) {
  const errno = binding.fs_ext_napi_try_downgrade_lock(fd, offset, length)

  if (errno < 0) {
    const err = toError(errno)
    if (err.code === 'EAGAIN') return false
    throw err
  }

  return true
}

exports.waitForDowngradeLock = function downgradeLock (fd, offset = 0, length = 0) {
  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_lock_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_wait_for_downgrade_lock(req, fd, offset, length, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

exports.tryUpgradeLock = function tryUpgradeLock (fd, offset = 0, length = 0) {
  const errno = binding.fs_ext_napi_try_upgrade_lock(fd, offset, length)

  if (errno < 0) {
    const err = toError(errno)
    if (err.code === 'EAGAIN') return false
    throw err
  }

  return true
}

exports.waitForUpgradeLock = function upgradeLock (fd, offset = 0, length = 0) {
  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_lock_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_wait_for_upgrade_lock(req, fd, offset, length, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

exports.unlock = function unlock (fd, offset = 0, length = 0) {
  const errno = binding.fs_ext_napi_unlock(fd, offset, length)

  if (errno < 0) throw toError(errno)
}

exports.trim = function trim (fd, offset, length) {
  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_trim_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_trim(req, fd, offset, length, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

exports.sparse = function sparse (fd) {
  // Short circuit on everything but Windows
  if (process.platform !== 'win32') return Promise.resolve()

  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_sparse_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_sparse(req, fd, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

exports.swap = function swap (from, to) {
  const req = Buffer.alloc(binding.sizeof_fs_ext_napi_swap_t)
  const ctx = {
    req,
    resolve: null,
    reject: null
  }

  const promise = new Promise((resolve, reject) => {
    ctx.resolve = resolve
    ctx.reject = reject
  })

  const errno = binding.fs_ext_napi_swap(req, from, to, ctx, onwork)

  if (errno < 0) return Promise.reject(toError(errno))

  return promise
}

function toError (errno) {
  const [code, msg] = errors.get(errno)

  const err = new Error(`${code}: ${msg}`)
  err.errno = errno
  err.code = code

  if (Error.captureStackTrace) {
    Error.captureStackTrace(err, toError)
  }

  return err
}
