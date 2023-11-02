#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <uv.h>
#include <string.h>
#include <stdlib.h>

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
  #if __FreeBSD_version > 1400029

    struct spacectl_range range = {
      .r_offset = offset,
      .r_len = length
    };

    return fspacectl(fd, SPACECTL_DEALLOC, &range, 0, &range);

  #else

    void *buf = calloc(1, length);

    ssize_t res = pwrite(fd, buf, length, offset);

    free(buf);

    if (res == -1) return uv_translate_sys_error(errno);

    return 0;

  #endif
}

int
fs_ext__swap (const char *from, const char *to) {

  char template[PATH_MAX];
  char *temp_name = "XXXXX";

  strcpy(template, P_tmpdir);
  strcat(template, temp_name);

  char *swap = mktemp(template);

  int err = rename(from, swap);
  if(err < 0) return err;

  err = rename(to, from);
  if(err < 0) return err;

  return rename(swap, to);
}
