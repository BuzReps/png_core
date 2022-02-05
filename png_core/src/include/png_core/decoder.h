#pragma once

#include "chunk_data.h"
#include "pixel_format.h"
#include "png_core.h"

extern const int8_t s_png_valid_bit_depths[5];
extern const int8_t s_png_valid_color_types[5];

struct PNGRawImage {
  enum PNGImageType type;
  void* data;
  int data_size;
  int scanline_pixel_count;
  int channel_bit_depth;
};
PNG_CORE_API void PNGInitRawImage(struct PNGRawImage* obj);

PNG_CORE_API int PNGGetPlainImageSizeBits(const struct PNGChunkData_IHDR* header);

PNG_CORE_API bool PNGGetRawImage(struct PNGRawChunk* chunk_list, struct PNGRawImage * out);

