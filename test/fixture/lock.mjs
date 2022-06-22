import { open } from 'fs/promises'
import minimist from 'minimist'
import { tryLock, waitForLock } from '../../index.js'

const argv = minimist(process.argv.slice(2), {
  boolean: ['shared'],
  string: ['mode', 'offset', 'length'],
  default: {
    mode: 'a+',
    shared: false,
    offset: 0,
    length: 0
  }
})

const offset = parseInt(argv.offset, 10)

const length = parseInt(argv.length, 10)

const options = {
  shared: argv.shared
}

const file = await open(argv._[0], argv.mode)

if (!tryLock(file.fd, offset, length, options)) {
  process.send({ granted: false })
}

await waitForLock(file.fd, offset, length, options)

process.send({ granted: true })

while (true) {
  //
}
