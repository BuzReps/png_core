#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "chunk_types.h"
#include "png_core.h"

/* Png datastream signature */
extern const uint8_t s_png_signature[8];
extern const int32_t s_png_max_chunk_data_length;

/* Function that constructs chunk data structure from buffer */
typedef void* (*PNGChunkDataStructLoadFunc)(const uint8_t* /* data */, int /* data_size */);

/*
* Function that writes chunk data into buffer 'out' in network-ordered format
* @note Call with NULL 'out' to get size
* @param[out] out Datastream buffer. Should contain enough space to contain chunk data. Can be NULL
* @return Amount of bytes written
*/
typedef int (*PNGChunkDataStructWriteFunc)(const void* /* data */, uint8_t* /* out */);

/* Function that frees chunk data structure */
typedef void (*PNGChunkDataStructFreeFunc)(void* /* data */);

struct PNGChunkDataStructFunctions {
  PNGChunkDataStructLoadFunc load_func;
  PNGChunkDataStructWriteFunc write_func;
  PNGChunkDataStructFreeFunc free_func;
};
PNG_CORE_API void PNGInitChunkDataStructFunctions(struct PNGChunkDataStructFunctions* obj);

/* Get functions that handles chunk data structures */
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
/*
 * Load IHDR chunk data from buffer 'data' into 'out'.
 * 'data' buffer is in network byte order
 */
PNG_CORE_API struct PNGChunkData_IHDR* PNGLoadData_IHDR(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_IHDR(const struct PNGChunkData_IHDR* data, void* out);
PNG_CORE_API void PNGFreeData_IHDR(struct PNGChunkData_IHDR* data);

/* tEXt */
struct PNGChunkData_tEXt {
  char* keyword;
  char* text;
};
PNG_CORE_API struct PNGChunkData_tEXt* PNGLoadData_tEXt(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_tEXt(const struct PNGChunkData_tEXt* data, void* out);
PNG_CORE_API void PNGFreeData_tEXt(struct PNGChunkData_tEXt* data);

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
PNG_CORE_API struct PNGChunkData_PLTE* PNGLoadData_PLTE(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_PLTE(const struct PNGChunkData_PLTE* data, void* out);
PNG_CORE_API void PNGFreeData_PLTE(struct PNGChunkData_PLTE* data);

/* bKGD */
struct PNGChunkData_bKGD {
  uint8_t index_in_palette;
};
PNG_CORE_API struct PNGChunkData_bKGD* PNGLoadData_bKGD(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_bKGD(const struct PNGChunkData_bKGD* data, void* out);
PNG_CORE_API void PNGFreeData_bKGD(struct PNGChunkData_bKGD* data);

/* gAMA */
struct PNGChunkData_gAMA {
  /*
   * The value representing gamma times 100000
   * Example: A gamma of 1/2.2 would be stored as the integer 45455
   */
  uint32_t gamma;
};
PNG_CORE_API struct PNGChunkData_gAMA* PNGLoadData_gAMA(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_gAMA(const struct PNGChunkData_gAMA* data, void* out);
PNG_CORE_API void PNGFreeData_gAMA(struct PNGChunkData_gAMA* data);

/* pHYs */
struct PNGChunkData_pHYs {
  uint32_t x_pixels_per_unit;
  uint32_t y_pixels_per_unit;
  uint8_t unit;
};
PNG_CORE_API struct PNGChunkData_pHYs* PNGLoadData_pHYs(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_pHYs(const struct PNGChunkData_pHYs* data, void* out);
PNG_CORE_API void PNGFreeData_pHYs(struct PNGChunkData_pHYs* data);

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
PNG_CORE_API struct PNGChunkData_sRGB* PNGLoadData_sRGB(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_sRGB(const struct PNGChunkData_sRGB* data, void* out);
PNG_CORE_API void PNGFreeData_sRGB(struct PNGChunkData_sRGB* data);

/* IDAT */
struct PNGChunkData_IDAT {
  uint8_t* data;
  int data_size;
};
PNG_CORE_API struct PNGChunkData_IDAT* PNGLoadData_IDAT(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_IDAT(const struct PNGChunkData_IDAT* data, void* out);
PNG_CORE_API void PNGFreeData_IDAT(struct PNGChunkData_IDAT* data);

/* sBIT */
struct PNGChunkData_sBIT {
  uint8_t bytes[4];
};
PNG_CORE_API struct PNGChunkData_sBIT* PNGLoadData_sBIT(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_sBIT(const struct PNGChunkData_sBIT* data, void* out);
PNG_CORE_API void PNGFreeData_sBIT(struct PNGChunkData_sBIT* data);

struct PNGChunkData_UnknownData {
  uint8_t* data;
  int data_size;
};
PNG_CORE_API struct PNGChunkData_UnknownData* PNGLoadData_UnknownData(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_UnknownData(const struct PNGChunkData_UnknownData* data, void* out);
PNG_CORE_API void PNGFreeData_UnknownData(struct PNGChunkData_UnknownData* data);

/* IEND */
struct PNGChunkData_IEND {
  uint8_t unused;
};
PNG_CORE_API struct PNGChunkData_IEND* PNGLoadData_IEND(const uint8_t* data, int data_size);
PNG_CORE_API int PNGWriteData_IEND(const struct PNGChunkData_IEND* data, void* out);
PNG_CORE_API void PNGFreeData_IEND(struct PNGChunkData_IEND* data);

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
