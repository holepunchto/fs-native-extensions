const fs = require('fs')

exports.open = function open(file, mode) {
  return new Promise((resolve, reject) => {
    fs.open(file, mode, (err, fd) => {
      if (err) reject(err)
      else resolve(fd)
    })
  })
}

exports.close = function close(fd) {
  return new Promise((resolve, reject) => {
    fs.close(fd, (err) => {
      if (err) reject(err)
      else resolve()
    })
  })
}

exports.stat = function stat(fd) {
  return new Promise((resolve, reject) => {
    fs.fstat(fd, (err, stat) => {
      if (err) reject(err)
      else resolve(stat)
    })
  })
}

exports.read = function read(fd, ...args) {
  return new Promise((resolve, reject) => {
    fs.read(fd, ...args, (err, result) => {
      if (err) reject(err)
      else resolve(result)
    })
  })
}

exports.write = function write(fd, ...args) {
  return new Promise((resolve, reject) => {
    fs.write(fd, ...args, (err, result) => {
      if (err) reject(err)
      else resolve(result)
    })
  })
}
