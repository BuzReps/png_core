#include "png_core/chunk_types.h"

const struct ChunkType CHUNK_INVALID = {.bytes = 0};

const struct ChunkType CHUNK_IHDR = {
    .byte1 = 73,
    .byte2 = 72,
    .byte3 = 68,
    .byte4 = 72,
};

const struct ChunkType CHUNK_sRGB = {
    .byte1 = 115,
    .byte2 = 82,
    .byte3 = 71,
    .byte4 = 66,
};

const struct ChunkType CHUNK_pHYs = {
    .byte1 = 112,
    .byte2 = 72,
    .byte3 = 89,
    .byte4 = 115,
};

const struct ChunkType CHUNK_tEXt = {
    .byte1 = 116,
    .byte2 = 69,
    .byte3 = 88,
    .byte4 = 116,
};

const struct ChunkType CHUNK_PLTE = {
    .byte1 = 80,
    .byte2 = 76,
    .byte3 = 84,
    .byte4 = 69,
};

const struct ChunkType CHUNK_bKGD = {
    .byte1 = 98,
    .byte2 = 75,
    .byte3 = 71,
    .byte4 = 68,
};

const struct ChunkType CHUNK_IEND = {
    .byte1 = 73,
    .byte2 = 69,
    .byte3 = 78,
    .byte4 = 68,
};

const struct ChunkType CHUNK_gAMA = {
    .byte1 = 103,
    .byte2 = 65,
    .byte3 = 77,
    .byte4 = 65,
};

const struct ChunkType CHUNK_IDAT = {
    .byte1 = 73,
    .byte2 = 68,
    .byte3 = 65,
    .byte4 = 84,
};

const struct ChunkType CHUNK_sBIT = {
    .byte1 = 115,
    .byte2 = 66,
    .byte3 = 73,
    .byte4 = 84,
};

bool IsValidChunkType(struct ChunkType type) {
  for (int i = 0; i < 4; ++i) {
    const uint8_t byte = type.byte_array[i];
    // A-Z
    if (byte >= 65 && byte <= 90)
      continue;
    // a-z
    if (byte >= 97 && byte <= 122)
      continue;
    return false;
  }
  // In this librarys supported standard, all chunk names shall have uppercase third letters
  if (IsChunkTypeReservedBitSet(type))
    return false;
  return true;
}
