#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <uv.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>

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
fs_ext__try_lock (uv_os_fd_t fd, uint64_t offset, size_t length, fs_ext_lock_type_t type) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, (type == FS_EXT_WRLOCK ? LOCK_EX : LOCK_SH) | LOCK_NB);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__unlock (uv_os_fd_t fd, uint64_t offset, size_t length) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, LOCK_UN);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

int
fs_ext__wait_for_lock (uv_os_fd_t fd, uint64_t offset, size_t length, fs_ext_lock_type_t type) {
  if (offset != 0 || length != 0) return UV_EINVAL;

  int res = flock(fd, type == FS_EXT_WRLOCK ? LOCK_EX : LOCK_SH);

  return res == -1 ? uv_translate_sys_error(errno) : res;
}

void random_string_len10(char *str){
  // Creates a 10 char long random alpha-numeric string (with in the input char array). Note the input array *str must have length of 11 to allow for 10 chars and a '\0' termination.
  const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  size_t charset_length = strlen(charset);
  size_t str_length = 10;

  for (size_t i = 0; i < str_length; ++i){
    str[i] = charset[arc4random_uniform(charset_length -1)];
  }
  str[str_length] = '\0';
}

int append_file_to_path_string(char *path, char *filename, char *result)
{
  // function to append a filename to directory name. Directory name must have a slash at the end.
  size_t path_length = strlen(path);
  size_t filename_length = strlen(filename);
  size_t result_size = path_length + filename_length + 1; // +1 for null terminator

  if (result_size > PATH_MAX ) {
      fprintf(stderr, "Cannot append file name to to path; result size is too large!\n");
      return EXIT_FAILURE;
  }

  strncpy(result, path, path_length);
  strncpy(result + path_length, filename, filename_length);
  result[result_size - 1] = '\0'; // Ensure null termination

  return EXIT_SUCCESS;
}

void append_slash(char *str) {
  // appends a slash to an input string
	size_t len = strlen(str);
	if (len > 0 && str[len -1] != '/' && len < PATH_MAX-1) {
		str[len] = '/';
		str[len+1] = '\0';
	}
}

int
swap_directories (const char *from, const char *to) {
  // *****************
  // Prep temporary directory with same underlying path as from
  char temp_dir[PATH_MAX];
  snprintf(temp_dir,sizeof(temp_dir), "%s.swap_temp", from);

  // *****************
  // Perform series of rename operations to swap to and from directories. 

  // 1. Rename fromdir to tempdir
  if( renameat( AT_FDCWD, from, AT_FDCWD, temp_dir) == -1) {
    printf("Renameat from - temp failed.\n");
    return uv_translate_sys_error(errno);  
  }  else {
    printf("Renameat from - to  worked!\n");
  }
  // 2. Rename todir to fromdir
  if( renameat( AT_FDCWD, to, AT_FDCWD, from) == -1) {
    printf("Renameat to - from failed.\n");
    return uv_translate_sys_error(errno);  
  }  else {
    printf("Renameat to - from  worked!\n");
  } 
  // 3. Rename temp_dir(now original fromdir) to todir
  if( renameat( AT_FDCWD, temp_dir, AT_FDCWD, to) == -1) {
    printf("Renameat temp - to failed.\n");
    if( renameat( AT_FDCWD, from, AT_FDCWD, to) == -1) {  printf("Rollback failed.\n");}
    return uv_translate_sys_error(errno);  
  }  else {
    printf("Renameat temp - to  worked!\n");
  }

  return 0;
}

int
swap_files (const char *from, const char *to) {

  // *****************
  // Prep temporary files with random names. Must share path of input files to avoid cross file system error.

  char temp_nameA[11];
  char temp_nameB[11];

  random_string_len10(temp_nameA);
  random_string_len10(temp_nameB);

  char *to_path = dirname(strdup(to));    
  size_t pathlen = strlen(to_path);

  if ( pathlen + 1 < PATH_MAX){
      append_slash(to_path);	
  } else {
    return -1;
  }

  char temp_fileA[PATH_MAX];
  char temp_fileB[PATH_MAX];

  append_file_to_path_string(to_path,temp_nameA,temp_fileA);
  append_file_to_path_string(to_path,temp_nameB,temp_fileB); 

  // *****************
  // Perform series of linking and unlinking operations to swap to and from file. "TO-file" and "FROM-file" in the comments denote the original underlying file objects.

  // 1. LINK (from,tempA) tempA and from can access FROM-file
  if( linkat( AT_FDCWD, from, AT_FDCWD, temp_fileA,0)== -1) {
    printf("link(from tempA) failed.\n");
    return uv_translate_sys_error(errno); 

  }
  else {
    printf("link(from tempA) worked!\n");
  }
  // 2. LINK (to,tempB) tempB and to can access TO-file
  if( linkat( AT_FDCWD, to, AT_FDCWD, temp_fileB,0)== -1) {
    printf("link(to tempB) failed.\n");
    return uv_translate_sys_error(errno);  
  } else {
    printf("link(to tempB) worked!\n");
  }
   // 3. UNLINK (from) only tempA can access FROM-file
  if( unlinkat( AT_FDCWD, from,0)== -1) {
    printf("unlink(from) failed.\n");
    return uv_translate_sys_error(errno); 
  } else {
    printf("unlink(from) worked!\n");
  }

  // 4. UNLINK (to) only tempB can access TO-file
  if( unlinkat( AT_FDCWD, to,0)== -1) {
    printf("unlink(to) failed.\n");
    return uv_translate_sys_error(errno); 
  }  else {
    printf("unlink(to) worked!\n");
  }

  // 5. LINK (tempA,to) tempA and to can access FROM-file
  if( linkat( AT_FDCWD, temp_fileA, AT_FDCWD, to,0)== -1) {
    printf("link(tempB to) failed.\n");
    return uv_translate_sys_error(errno);  
  }  else {
    printf("link(to tempB) worked!\n");
  }

  // 6. LINK (tempB,from) tempB and from can access TO-file
  if( linkat( AT_FDCWD, temp_fileB, AT_FDCWD, from,0)== -1) {
    printf("link(tempB from) failed.\n");
    return uv_translate_sys_error(errno);  
  } else {
    printf("link(tempB,from) worked!\n");
  }

  // 7. UNLINK (tempA) only to can access FROM-file
  if( unlinkat( AT_FDCWD, temp_fileA,0)== -1) {
    printf("unlink(tempA) failed.\n");
    return uv_translate_sys_error(errno); 
  } else {
    printf("unlink(tempA) worked!\n");
  }

  // 8. UNLINK (tempB) only from can access TO-file
  if( unlinkat( AT_FDCWD, temp_fileB,0)== -1) {
    printf("unlink(tempB) failed.\n");
    return uv_translate_sys_error(errno); 
  } else {
    printf("unlink(tempB) worked!\n");
  }

  return 0;
}

int
fs_ext__swap (const char *from, const char *to) {
  // use sys/stat.h to determine if from and to are files or directories
  struct stat st_from, st_to;

  int from_is_dir = stat(from, &st_from) == 0 && S_ISDIR(st_from.st_mode);
  int to_is_dir = stat(to, &st_to) == 0 && S_ISDIR(st_to.st_mode);

  // Call swap_files or swap_directories dependendent whether inputs are files or directories:
  switch (from_is_dir * 2 + to_is_dir) {
    case 0: // Both are files
      if( swap_files(from,to)!=0){ return uv_translate_sys_error(errno);} // swap files
      return 0;
    case 1: // from is file to is dir: file dir-swap seems to work with case 3 code, so no return statements included.
    case 2: // from is dir to is file
    case 3: // Both are dirs
      if (swap_directories(from,to)!=0){  return uv_translate_sys_error(errno);}
      return 0; 
    default: // something else:  ERR
      return -1;
    }
}

static inline size_t
fs_ext__align_up (size_t n, size_t align) {
  return (n + (align - 1)) & -align;
}

static inline size_t
fs_ext__align_down (size_t n, size_t align) {
  return n & -align;
}

int
fs_ext__trim (uv_os_fd_t fd, uint64_t offset, size_t length) {
  printf("fs_ext__trim? \n");
  
  // get file size:
  struct stat st;
  if (fstat(fd, &st) == -1) {
	  perror("Failed to get file size");
	  return uv_translate_sys_error(errno);
  }
  
  size_t block_size = st.st_blksize;
  off_t aligned_offset = fs_ext__align_down(offset, block_size); //offset - (offset % block_size);
  off_t aligned_end = fs_ext__align_up(offset + length, block_size); 
  off_t aligned_length = aligned_end - aligned_offset;

  if (aligned_offset != (long long)offset || aligned_length != (long long)length) {
	  // Unaligned blocks

	  if (ftruncate(fd, aligned_offset) == -1) {
		  return uv_translate_sys_error(errno);
	  }

	  if (ftruncate(fd, aligned_end) == -1) {
		  return uv_translate_sys_error(errno);
	  }

	  if (ftruncate(fd, aligned_end) == -1) {
		  return uv_translate_sys_error(errno);
	  }

	  return 0;
  } else {
    // Aligned blocks:

    // calc post trim size:
    off_t new_size = st.st_size - length;
        // create temporary buffer for trimmed file contents:
    char* buffer = malloc(new_size);
    if (buffer == NULL) {
    perror("Failed to allocate memory");
        return -1;
    }

	  // read file contents into buffer
	  ssize_t num_read = pread(fd, buffer, new_size,length);
 	  if (num_read == -1) {
	  	perror("Failed to read file");
	  	free(buffer);
	  	return -1;
  	}

	  // move pointer back to start of file:
	  if (lseek(fd, 0, SEEK_SET) == -1) {
	  	perror("Failed to seek file");
	  	free(buffer);
	  	return -1;
  	}

    //Write the modified buffer back to the file
    ssize_t num_written = write(fd, buffer, num_read);
    if (num_written == -1) {
      perror("Failed to write file");
      free(buffer);
      return -1;
    }

    // Truncate the file to trimmed size:
    if (ftruncate(fd, new_size) == -1) {
      perror("Failed to truncate file");
      free(buffer);
      return -1;
    }

    // tidy up - free the buffer:
    free(buffer);
    return 0;
  }
}

