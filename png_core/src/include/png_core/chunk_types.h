#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ChunkType {
  union {
    struct {
      uint8_t byte1;
      uint8_t byte2;
      uint8_t byte3;
      uint8_t byte4;
    };
    uint32_t bytes;
    uint8_t byte_array[4];
  };
};

extern const struct ChunkType CHUNK_INVALID;
extern const struct ChunkType CHUNK_IHDR;
extern const struct ChunkType CHUNK_sRGB;
extern const struct ChunkType CHUNK_pHYs;
extern const struct ChunkType CHUNK_tEXt;
extern const struct ChunkType CHUNK_PLTE;
extern const struct ChunkType CHUNK_bKGD;
extern const struct ChunkType CHUNK_IEND;
extern const struct ChunkType CHUNK_gAMA;
extern const struct ChunkType CHUNK_IDAT;
extern const struct ChunkType CHUNK_sBIT;

/*
 * Checks is bytes are valid chunk type
 */
PNG_CORE_API bool IsValidChunkType(struct ChunkType type);

/*
 * If bit is set, chunk is ancillary. Critical otherwise
 */
inline bool IsChunkTypeAncillary(struct ChunkType type) {
  return type.byte1 | (1 << 5);
}

/*
 * If bit is set, chunk is private. Public otherwise
 */
inline bool IsChunkTypePrivate(struct ChunkType type) {
  return type.byte2 | (1 << 5);
}

/*
 * Reserved bit should be always unset in this PNG version
 */
inline bool IsChunkTypeReservedBitSet(struct ChunkType type) {
  return type.byte3 | (1 << 5);
}

inline bool IsChunkTypeSafeToCopy(struct ChunkType type) {
  return type.byte4 | (1 << 5);
}

#ifdef __cplusplus
}  // extern "C"
#endif
