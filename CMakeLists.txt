cmake_minimum_required(VERSION 3.25)

project(fs_native_extensions C)

include(pear)

add_pear_module(fs_native_extensions)

target_sources(
  fs_native_extensions
  INTERFACE
    include/fs-ext.h
  PRIVATE
    binding.c
    src/shared.c
)

if(UNIX)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/posix.c
  )
endif()

if(APPLE)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/mac.c
  )
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_sources(
    fs_native_extensions
    PRIVATE
      src/linux.c
  )
endif()

if(WIN32)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/win.c
  )
endif()
