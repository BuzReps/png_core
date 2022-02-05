#include "png_core/compression.h"

#include <zlib.h>

#include <stdlib.h>

uint8_t* PNGDataDecompress0(const uint8_t* compressed, int compressed_size, int decompressed_size) {
  uint8_t* decompressed = malloc(decompressed_size);
  uLongf out_size = decompressed_size;
  const int ret = uncompress(decompressed, &out_size, compressed, compressed_size);
  if (Z_OK != ret) {
    free(decompressed);
    return NULL;
  }
  return decompressed;
}

PNGDataDecompressionFunction PNGGetDataDecompressionFunction(uint8_t compression_method) {
  switch (compression_method) {
    case PNG_COMPRESSION_METHOD_0: return PNGDataDecompress0;
    default: return NULL;
  }
}

uint8_t* PNGDataDecompress(uint8_t method, const uint8_t* compressed, int compressed_size, int decompressed_length) {
  if (method == PNG_COMPRESSION_METHOD_0)
    return PNGDataDecompress0(compressed, compressed_size, decompressed_length);
  return NULL;
}

