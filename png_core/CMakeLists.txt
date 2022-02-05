set(target_name png_core)

add_library(${target_name} SHARED)
add_compile_definitions(PNG_CORE_EXPORT)

# library own public header files
target_include_directories(${target_name} PUBLIC src/include)
# library own private header files
target_include_directories(${target_name} PRIVATE src)

# library source files
set(SOURCES_LIST
	src/chunk_data.c
	src/chunk_types.c
	src/compression.c
	src/datastream.c
	src/decoder.c
	src/filtering.c
	src/pixel_format.c
	src/tools.c
)

target_sources(${target_name} PRIVATE ${SOURCES_LIST})

target_link_libraries(${target_name} PRIVATE ZLIB::ZLIB)

