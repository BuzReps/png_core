# BuzReps/png_explorer

A C11 library and a GUI tool for [PNG](https://www.w3.org/TR/png-3/) images editing

## Project status

Nothing to see here. Work In Progress

## Dependencies and tools

Running tasks: [Make](Makefile)  
Build tool: [CMake](https://cmake.org)  
Dependency manager: [vcpkg](https://vcpkg.io)  
PNG's compressing algorithm: [zlib](https://www.zlib.net) (managed by vcpkg)  
Documentation generation: [Doxygen](https://www.doxygen.nl) [optional]  
Testing framework: [GoogleTest](https://github.com/google/googletest) [optional]  
Code formatting: [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) [optional]  

## Setup, building and workflow

### Windows

Visual studio should do the job

### Linux:

Run `make init` once to setup project  
Run `make configure` to configure and generate build targets  
Run `make build` to build project  
Run `make clean` to remove all build files and artifacts  
