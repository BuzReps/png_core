OUT_DIR := out
INSTALL_DIR := install
DOCS_DIR := documentation

help:
	@echo "make help - Print this message"
	@echo "make init - Project initialization"
	@echo "make configure - Rescan project"
	@echo "make build - Build project"
	@echo "make clean - Remove all build files"
	@echo "make generate_docs - Generate documentation"
	@echo "make run_tests - Run tests"

# First time project initialization
init:
	./vcpkg/bootstrap-vcpkg.sh -disableMetrics
	# For clangd users: compilation database file into project root
	# Windows: exec 'copy "out\build\<triplet>\compile_commands.json" .'
	# after each build
	ln -s ./out/compile_commands.json .

configure:
	cmake -S . -B $(OUT_DIR) -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"

build:
	cmake --build $(OUT_DIR)
	cmake --install $(OUT_DIR) --prefix $(INSTALL_DIR)

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(INSTALL_DIR)
	rm -rf $(DOCS_DIR)

generate_docs:
	doxygen Doxyfile

run_tests:
	# TODO
	$(error Not implemented)

