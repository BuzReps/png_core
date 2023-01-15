/**
 * @file png_core/chunk_data.h
 *
 * @author Vladislav Buzun
 *
 * @date 15 Jan 2023
 *
 * @brief File containing chunk data structures and helping functions
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "chunk_types.h"
#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Png datastream signature
 */
extern const uint8_t s_png_signature[8];

/**
 * Max chunk data size in bytes.
 */
extern const int32_t s_png_max_chunk_data_size_bytes;


/**
 * Type of function that constructs chunk data structure of specifc type
 * @param[in] raw_data Network-ordered chunk raw data (not whole chunk), not null
 * @param[in] size_bytes raw_data size in bytes
 * @return Constructed data object or NULL, if error occurred.
 *   Caller is responsible for freeing it with corresponding PNGChunkDataStructFreeFunc function
 */
typedef void* (*PNGChunkDataStructLoadFunc)(const uint8_t* /* raw_data */, int /* size_bytes */);

/**
 * Function that writes chunk data into network-ordered buffer
 * @note Call with `out == NULL` to get required size in bytes
 * @param[in] obj Chunk data structure object, not NULL
 * @param[out] out Datastream buffer. Should contain enough space to contain chunk data. Can be NULL
 * @return Amount of bytes written
 */
typedef uint32_t (*PNGChunkDataStructWriteFunc)(const void* /* obj */, uint8_t* /* out */);

/**
 * Function that frees chunk data structure object
 * @param[in] obj Chunk data structure object, nullable
 */
typedef void (*PNGChunkDataStructFreeFunc)(void* /* obj */);

/**
 * Declare Construct/Write/Free functions for chunk data structure.
 * See PNGChunkDataStructLoadFunc, PNGChunkDataStructWriteFunc, PNGChunkDataStructFreeFunc.
 * @example for PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(IHDR):
 *   PNG_CORE_API struct PNGChunkData_IHDR* PNGLoadData_IHDR(const uint8_t* data, int data_size);
 *   PNG_CORE_API int PNGWriteData_IHDR(const struct PNGChunkData_IHDR* data, void* out);
 *   PNG_CORE_API void PNGFreeData_IHDR(struct PNGChunkData_IHDR* data, void* out);
 */
#define PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(chunk_type)                                                    \
  PNG_CORE_API struct PNGChunkData_##chunk_type* PNGLoadData_##chunk_type(const uint8_t* data, int data_size); \
  PNG_CORE_API int PNGWriteData_##chunk_type(const struct PNGChunkData_##chunk_type* obj, void* out);          \
  PNG_CORE_API void PNGFreeData_##chunk_type(struct PNGChunkData_##chunk_type* obj);

/**
 * Function set for chunk data struct
 */
struct PNGChunkDataStructFunctions {
  PNGChunkDataStructLoadFunc load_func;
  PNGChunkDataStructWriteFunc write_func;
  PNGChunkDataStructFreeFunc free_func;
};
PNG_CORE_API void PNGInitChunkDataStructFunctions(struct PNGChunkDataStructFunctions* obj);

/**
 * Get function set for chunk data struct of specific type
 * @param[in] type Chunk type
 * @return Function set for chunk data type.
 */
struct PNGChunkDataStructFunctions PNGGetChunkDataStructFunctions(struct ChunkType type);

/* IHDR */
struct PNGChunkData_IHDR {
  int32_t width;
  int32_t height;
  int8_t bit_depth;
  int8_t color_type;
  int8_t compression_method;
  int8_t filter_method;
  int8_t interlace_method;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(IHDR)

/* tEXt */
struct PNGChunkData_tEXt {
  char* keyword;
  char* text;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(tEXt)

struct PaletteDataEntry {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

/* PLTE */
struct PNGChunkData_PLTE {
  struct PaletteDataEntry* entries;
  int entries_count;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(PLTE)

/* bKGD */
struct PNGChunkData_bKGD {
  uint8_t index_in_palette;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(bKGD)

/* gAMA */
struct PNGChunkData_gAMA {
  /*
   * The value representing gamma times 100000
   * Example: A gamma of 1/2.2 would be stored as the integer 45455
   */
  uint32_t gamma;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(gAMA)

/* pHYs */
struct PNGChunkData_pHYs {
  uint32_t x_pixels_per_unit;
  uint32_t y_pixels_per_unit;
  uint8_t unit;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(pHYs)

enum PNGRenderingIntent {
  /*
   * For images preferring good adaptation to the output device gamut
   * at the expense of colorimetric accuracy, such as photographs.
   */
  PNG_RENDERING_INTENT_PERCEPTUAL = 0,
  /*
   * For images requiring colour appearance matching
   * (relative to the output device white point), such as logos.
   */
  PNG_RENDERING_INTENT_RELATIVE_COLORIMETRIC = 1,
  /*
   * For images preferring preservation of saturation at the expense
   * of hue and lightness, such as charts and graphs.
   */
  PNG_RENDERING_INTENT_SATURATION = 2,
  /*
   * For images requiring preservation of absolute colorimetry,
   * such as previews of images destined for a different output device (proofs).
   */
  PNG_RENDERING_INTENT_ABSOLUTE_COLORIMETRIC = 3,
};

/* sRGB */
struct PNGChunkData_sRGB {
  uint8_t rendering_intent;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(sRGB)

/* IDAT */
struct PNGChunkData_IDAT {
  uint8_t* data;
  int data_size;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(IDAT)

/* sBIT */
struct PNGChunkData_sBIT {
  uint8_t bytes[4];
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(sBIT)

struct PNGChunkData_UnknownData {
  uint8_t* data;
  int data_size;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(UnknownData)

/* IEND */
struct PNGChunkData_IEND {
  uint8_t unused;
};
PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS(IEND)

#undef PNG_DECLARE_CHUNK_DATA_STRUCT_FUNCTIONS

struct PNGRawChunk {
  /* Chunk raw data length in bytes */
  uint32_t length;
  struct ChunkType type;
  /* Pointer to chunk raw data */
  uint8_t* raw_data;
  /* Pointer to chunk-specific parsed data structire */
  void* parsed_data;
  /*
   * A Cyclic Redundancy Code calculated
   * on the chunk type field and chunk data fields
   */
  uint32_t crc;

  struct PNGRawChunk* next;
};

PNG_CORE_API void PNGInitRawChunk(struct PNGRawChunk* obj);

PNG_CORE_API void PNGFreeRawChunkList(struct PNGRawChunk* node);

#ifdef __cplusplus
}  // extern "C"
#endif
