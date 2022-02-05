set(target_name png_cli)

add_executable(${target_name} SHARED)
target_link_libraries(${target_name} PRIVATE png_core)

set(SOURCES_LIST
	src/main.cpp
)

target_sources(${target_name} PRIVATE ${SOURCES_LIST})
