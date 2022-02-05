#include "png_core/decoder.h"
#include "png_core/compression.h"
#include "png_core/filtering.h"

#include <memory.h>
#include <stdlib.h>
#include <math.h>

const int8_t s_png_valid_bit_depths[5] = {1, 2, 4, 8, 16};
const int8_t s_png_valid_color_types[5] = {0, 2, 3, 4, 6};

static bool ConcatenateAllIDATChunks(const struct PNGRawChunk* chunk_list, uint8_t ** out, int * out_size) {
  const struct PNGRawChunk* chunk = chunk_list;

  int concat_total_size = 0;
  while (chunk) {
    if (chunk->type.bytes == CHUNK_IDAT.bytes) {
      if (chunk->parsed_data) {
        const struct PNGChunkData_IDAT* data = (struct PNGChunkData_IDAT*)chunk->parsed_data;
        concat_total_size += data->data_size;
      }
    }
    chunk = chunk->next;
  }

  chunk = chunk_list;
  uint8_t* concatenated_idat = malloc(concat_total_size);
  uint8_t* ptr = concatenated_idat;

  if (!concatenated_idat)
    return false;

  while (chunk) {
    if (chunk->type.bytes == CHUNK_IDAT.bytes) {
      if (chunk->parsed_data) {
        const struct PNGChunkData_IDAT* data = (struct PNGChunkData_IDAT*)chunk->parsed_data;
        memcpy(ptr, data->data, data->data_size);
        ptr += data->data_size;
      }
    }
    chunk = chunk->next;
  }

  *out = concatenated_idat;
  *out_size = concat_total_size;
  return true;
}

void PNGInitRawImage(struct PNGRawImage* obj) {
  memset(obj, 0, sizeof(struct PNGRawImage));
}

int PNGGetPlainImageSizeBits(const struct PNGChunkData_IHDR* header) {
  const int channels_in_px = PNGGetChannelCount(header->color_type);
  const int bits_in_px = channels_in_px * header->bit_depth;
  const int bits_in_total_image = header->width * header->height * bits_in_px;
  return bits_in_total_image;
}

bool PNGGetRawImage(struct PNGRawChunk* chunk_list, struct PNGRawImage * out) {
  uint8_t* idat_concated = NULL;
  int idat_concated_size = 0;

  if (!ConcatenateAllIDATChunks(chunk_list, &idat_concated, &idat_concated_size))
    return false;

  const struct PNGChunkData_IHDR* header = NULL;
  struct PNGRawChunk* chunk = chunk_list;
  while (chunk) {
    if (!header && chunk->type.bytes == CHUNK_IHDR.bytes) {
      header = (struct PNGChunkData_IHDR*)chunk->parsed_data;
      break;
    }
    chunk = chunk->next;
  }
  if (!header) {
    free(idat_concated);
    return false;
  }

  const PNGDataDecompressionFunction decompression_func = PNGGetDataDecompressionFunction(header->compression_method);
  if (!decompression_func) {
    free(idat_concated);
    return false;
  }
  const int filtered_size = PNGGetFilteredImageSizeBytes(header);
  uint8_t* decompressed = decompression_func(idat_concated, idat_concated_size, filtered_size);
  free(idat_concated);
  if (!decompressed)
    return false;

  const PNGDataDefilteringFunction defiltering_function = PNGGetDefilteringFunction(header->filter_method);
  if (!defiltering_function) {
    free(decompressed);
    return false;
  }
  const int plain_size_bytes = ceil(PNGGetPlainImageSizeBits(header) / 8.0);
  uint8_t* plain_data = malloc(plain_size_bytes);
  if (!plain_data) {
    free(decompressed);
    return false;
  }
  const int pixel_size_bytes = PNGGetChannelCount(header->color_type) * header->bit_depth;
  if (!defiltering_function(decompressed, filtered_size, header->width, pixel_size_bytes, plain_data)) {
    free(decompressed);
    return false;
  }
  free(decompressed);

  PNGInitRawImage(out);
  out->type = header->color_type;
  out->data = plain_data;
  out->data_size = plain_size_bytes;
  out->scanline_pixel_count = header->width;
  out->channel_bit_depth = header->bit_depth;

  return true;
}

