cmake_minimum_required(VERSION 3.22)

# Print this message just to use variable CMAKE_TOOLCHAIN_FILE
# Because this variable, passed in command line during second and subsequent
# reconfigurations, causes annoying warning
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

project(
  PNGExplorer
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Generate compile-commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Make 'cmake/*.cmake' modules visible for `include(...)` function
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Build targets
find_package(ZLIB REQUIRED)
add_subdirectory(png_core)

# Testing
find_package(GTest)
if(GTest_FOUND)
  enable_testing()
  add_subdirectory(tests)
endif()

# Documentation generation targets
include(generate_docs) # cmake/generate_docs.cmake
AppendToDocsTarget(png_core)

# Installing and packaging targets
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
include(GNUInstallDirs)
install(TARGETS png_core
  EXPORT png_core
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

