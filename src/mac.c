#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <uv.h>

#include "../include/fs-ext.h"
#include "platform.h"

int
fs_ext__try_lock (uv_os_fd_t fd, uint64_t offset, size_t length, fs_ext_lock_type_t type) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, (type == FS_EXT_WRLOCK ? LOCK_EX : LOCK_SH) | LOCK_NB);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__wait_for_lock (uv_os_fd_t fd, uint64_t offset, size_t length, fs_ext_lock_type_t type) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, type == FS_EXT_WRLOCK ? LOCK_EX : LOCK_SH);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__unlock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, LOCK_UN);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__trim (uv_os_fd_t fd, uint64_t offset, size_t length) {
  struct fpunchhole data = {
    .fp_flags = 0,
    .reserved = 0,
    .fp_offset = offset,
    .fp_length = length,
  };

  int res = fcntl(fd, F_PUNCHHOLE, &data);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__swap (const char *from, const char *to) {
  int res = renameatx_np(AT_FDCWD, from, AT_FDCWD, to, RENAME_SWAP);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}
