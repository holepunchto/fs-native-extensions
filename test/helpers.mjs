import fs from 'fs'

export function open (file, mode) {
  return new Promise((resolve, reject) => {
    fs.open(file, mode, (err, fd) => {
      if (err) reject(err)
      else resolve(fd)
    })
  })
}

export function close (fd) {
  return new Promise((resolve, reject) => {
    fs.close(fd, (err) => {
      if (err) reject(err)
      else resolve()
    })
  })
}

export function stat (fd) {
  return new Promise((resolve, reject) => {
    fs.fstat(fd, (err, stat) => {
      if (err) reject(err)
      else resolve(stat)
    })
  })
}

export function read (fd, ...args) {
  return new Promise((resolve, reject) => {
    fs.read(fd, ...args, (err, result) => {
      if (err) reject(err)
      else resolve(result)
    })
  })
}

export function write (fd, ...args) {
  return new Promise((resolve, reject) => {
    fs.write(fd, ...args, (err, result) => {
      if (err) reject(err)
      else resolve(result)
    })
  })
}
