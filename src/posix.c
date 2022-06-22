#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <uv.h>

#include "../include/fs-ext.h"
#include "platform.h"

int
fs_ext__try_downgrade_lock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  return fs_ext__try_lock(fd, offset, length, FS_EXT_RDLOCK);
}

int
fs_ext__wait_for_downgrade_lock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  return fs_ext__wait_for_lock(fd, offset, length, FS_EXT_RDLOCK);
}

int
fs_ext__try_upgrade_lock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  return fs_ext__try_lock(fd, offset, length, FS_EXT_WRLOCK);
}

int
fs_ext__wait_for_upgrade_lock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  return fs_ext__wait_for_lock(fd, offset, length, FS_EXT_WRLOCK);
}

int
fs_ext__sparse (uv_os_fd_t fd) {
  return 0;
}

int
fs_ext__swap (const char *from_path, const char *to_path) {
  return fs_ext__swap_at(AT_FDCWD, from_path, AT_FDCWD, to_path);
}
