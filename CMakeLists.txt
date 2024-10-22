cmake_minimum_required(VERSION 3.25)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-napi REQUIRED PATHS node_modules/cmake-napi)

project(fs_native_extensions C)

bare_target(target)

set(sources binding.c src/shared.c)

if(NOT target MATCHES "win32")
  list(APPEND sources src/posix.c)
endif()

if(target MATCHES "darwin|ios")
  list(APPEND sources src/mac.c)
endif()

if(target MATCHES "linux|android")
  list(APPEND sources src/linux.c)
endif()

if(target MATCHES "win32")
  list(APPEND sources src/win.c)
endif()

add_bare_module(fs_native_extensions_bare)

target_sources(
  ${fs_native_extensions_bare}
  INTERFACE
    include/fs-ext.h
  PRIVATE
    ${sources}
)

add_napi_module(fs_native_extensions_node)

target_sources(
  ${fs_native_extensions_node}
  INTERFACE
    include/fs-ext.h
  PRIVATE
    ${sources}
)
