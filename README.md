# BuzReps/png_explorer

`png_core` - A C11 library for [PNG](https://www.w3.org/TR/png-3/) images manipulation  
`png_explorer` - A small PNG image editor  

## Project status

Nothing to see here. Work In Progress. Come back later.

## Used tools and dependencies

* Running tasks: [Make](Makefile)*  
* Build tool: [CMake](https://cmake.org)*  
* Dependency manager: [vcpkg](https://vcpkg.io)  
* `png_core`'s dependency: PNG's compressing algorithm [zlib](https://www.zlib.net)  
* Testing framework: [GoogleTest](https://github.com/google/googletest)  
* Documentation generation: [Doxygen](https://www.doxygen.nl)**  
  * Also [GraphWiz](https://graphviz.org/) for graphs generation**  
* Code formatting: [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html)**  

\* - Installed manually  
\** - Optional, installed manually  

## Setup, building and workflow

### Windows

Visual studio should do the job

### Linux:

* Run `make init` once to setup project  
* Run `make configure` to configure and generate build targets  
* Run `make build` to build project  
  * Artifacts will appear in `install` folder  
* Run `make clean` to remove all generated files and artifacts  
* Run `make generate_docs` to generate documentation  
  * Docs will appear in `documentation` folder  
  * Open `documentation/html/index.html` in browser  

