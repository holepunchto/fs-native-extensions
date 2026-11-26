cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-fetch REQUIRED PATHS node_modules/cmake-fetch)
find_package(cmake-napi REQUIRED PATHS node_modules/cmake-napi)

project(fs_native_extensions C)

fetch_package("github:holepunchto/libnapish#8cbfe78")
fetch_package("github:libuv/libuv@1.51.0")

add_library(fs_native_extensions STATIC)

target_sources(
  fs_native_extensions
  INTERFACE
    include/fs-ext.h
  PRIVATE
    src/platform.h
    src/shared.c
)

target_include_directories(
  fs_native_extensions
  INTERFACE
    include
  PRIVATE
    $<TARGET_PROPERTY:uv,INTERFACE_INCLUDE_DIRECTORIES>
)

if(APPLE)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/apple.c
  )
endif()

if(LINUX OR ANDROID)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/linux.c
  )

  target_compile_definitions(
    fs_native_extensions
    PRIVATE
      _GNU_SOURCE
      _FILE_OFFSET_BITS=64
  )
endif()

if(WIN32)
  target_sources(
    fs_native_extensions
    PRIVATE
      src/win32.c
      src/win32/nt.h
  )
else()
  target_sources(
    fs_native_extensions
    PRIVATE
      src/posix.c
  )
endif()

add_bare_module(fs_native_extensions_bare INSTALL PRIVACY_MANIFEST PrivacyInfo.xcprivacy)

target_sources(
  ${fs_native_extensions_bare}
  PRIVATE
    binding.c
)

target_link_libraries(
  ${fs_native_extensions_bare}
  PRIVATE
    fs_native_extensions
)

if(WIN32)
  target_link_libraries(
    ${fs_native_extensions_bare}
    PUBLIC
      ntdll
  )
endif()

add_napi_module(fs_native_extensions_napi)

target_sources(
  ${fs_native_extensions_napi}
  PRIVATE
    binding.c
)

target_link_libraries(
  ${fs_native_extensions_napi}
  PRIVATE
    fs_native_extensions
    napish::static
)

if(WIN32)
  target_link_libraries(
    ${fs_native_extensions_napi}
    PUBLIC
      ntdll
  )
endif()
