#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

enum PNGCompressionMethod {
  PNG_COMPRESSION_METHOD_0 = 0
};

/*
 * Decompress using compression method 0
 */
PNG_CORE_API uint8_t* PNGDataDecompress0(const uint8_t* compressed, int compressed_size, int decompressed_size);

typedef uint8_t* (*PNGDataDecompressionFunction)(const uint8_t* compressed, int compressed_size, int decompressed_size);

PNG_CORE_API PNGDataDecompressionFunction PNGGetDataDecompressionFunction(uint8_t compression_method);

#ifdef __cplusplus
}  // extern "C"
#endif
