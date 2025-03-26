const minimist = require('minimist')
const { open } = require('../helpers')
const { tryLock, waitForLock } = require('../..')

async function main() {
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

  const fd = await open(argv._[0], argv.mode)

  if (!tryLock(fd, offset, length, options)) {
    process.send({ granted: false })
  }

  await waitForLock(fd, offset, length, options)

  process.send({ granted: true })

  while (true) {
    //
  }
}

main()
