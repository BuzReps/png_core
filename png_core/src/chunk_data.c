#include "png_core/chunk_data.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "tools.h"

const uint8_t s_png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
const int32_t s_png_max_chunk_data_length = INT32_MAX;

void PNGInitChunkDataStructFunctions(struct PNGChunkDataStructFunctions *obj) {
  obj->load_func = NULL;
  obj->write_func = NULL;
  obj->free_func = NULL;
}

struct PNGChunkDataStructFunctions PNGGetChunkDataStructFunctions(struct ChunkType type) {
#define PNG_MAKE_FUNCTIONS(chunk_name)                \
  if (type.bytes == CHUNK_##chunk_name.bytes) {       \
    struct PNGChunkDataStructFunctions functions;     \
    PNGInitChunkDataStructFunctions(&functions);      \
    functions.load_func = (PNGChunkDataStructLoadFunc)PNGLoadData_##chunk_name;    \
    functions.write_func = (PNGChunkDataStructWriteFunc)PNGWriteData_##chunk_name; \
    functions.free_func = (PNGChunkDataStructFreeFunc)PNGFreeData_##chunk_name;    \
    return functions;                                 \
  }
  PNG_MAKE_FUNCTIONS(IHDR)
  PNG_MAKE_FUNCTIONS(sRGB)
  PNG_MAKE_FUNCTIONS(pHYs)
  PNG_MAKE_FUNCTIONS(tEXt)
  PNG_MAKE_FUNCTIONS(PLTE)
  PNG_MAKE_FUNCTIONS(bKGD)
  PNG_MAKE_FUNCTIONS(IEND)
  PNG_MAKE_FUNCTIONS(gAMA)
  PNG_MAKE_FUNCTIONS(IDAT)
  PNG_MAKE_FUNCTIONS(sBIT)
#undef PNG_MAKE_FUNCTIONS

  struct PNGChunkDataStructFunctions functions;
  PNGInitChunkDataStructFunctions(&functions);
  return functions;
}

struct PNGChunkData_IHDR *PNGLoadData_IHDR(const uint8_t *data, int data_size) {
  if (data_size != 13)
    return NULL;

  struct PNGChunkData_IHDR *out = malloc(sizeof(struct PNGChunkData_IHDR));
  if (!out)
    return NULL;

  out->width = *(const int32_t *)data;
  data += sizeof(int32_t);
  out->height = *(const int32_t *)data;
  data += sizeof(int32_t);
  memcpy(&out->bit_depth, data, 5);

  if (PNG_IS_LITTLE_ENDIAN) {
    InvertBytes(&out->width, sizeof(int32_t));
    InvertBytes(&out->height, sizeof(int32_t));
  }

  return out;
}

int PNGWriteData_IHDR(const struct PNGChunkData_IHDR *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_IHDR(struct PNGChunkData_IHDR *data) {
  free(data);
}

struct PNGChunkData_tEXt *PNGLoadData_tEXt(const uint8_t *data, int data_size) {
  if (data_size < 1)
    return NULL;

  struct PNGChunkData_tEXt *out = malloc(sizeof(struct PNGChunkData_tEXt));
  if (!out)
    return NULL;

  int null_idx = -1;
  for (int i = 0; i < data_size; ++i) {
    if (data[i] == '\0') {
      null_idx = i;
      break;
    }
  }
  if (null_idx == -1 || null_idx == 0) {
    free(out);
    return NULL;
  }

  // 0  1  2  3  4  5  6   7  8  9  10 11 12
  //[k][e][y][w][o][r][\0][t][e][x][x][ ][ ]

  // string widths including null-terminator
  const int keyword_sz_length = null_idx + 1;
  const int text_sz_length = data_size - null_idx - 1 + 1;
  out->keyword = malloc(keyword_sz_length);
  if (!out->keyword) {
    free(out);
    return NULL;
  }
  out->text = malloc(text_sz_length);
  if (!out->text) {
    free(out->keyword);
    free(out);
    return NULL;
  }

  memcpy(out->keyword, data, keyword_sz_length - 1);
  out->keyword[keyword_sz_length - 1] = '\0';
  memcpy(out->text, &data[null_idx + 1], text_sz_length - 1);
  out->text[text_sz_length - 1] = '\0';

  return out;
}

int PNGWriteData_tEXt(const struct PNGChunkData_tEXt *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_tEXt(struct PNGChunkData_tEXt *data) {
  free(data->keyword);
  free(data->text);
  free(data);
}

struct PNGChunkData_PLTE *PNGLoadData_PLTE(const uint8_t *data, int data_size) {
  if (data_size % 3 != 0)
    return NULL;

  struct PNGChunkData_PLTE *out = malloc(sizeof(struct PNGChunkData_PLTE));
  if (!out)
    return NULL;

  out->entries_count = data_size / 3;
  out->entries = malloc(out->entries_count * sizeof(struct PaletteDataEntry));
  if (!out->entries) {
    free(out);
    return NULL;
  }

  for (int i = 0; i < out->entries_count; ++i) {
    struct PaletteDataEntry *entry = &out->entries[i];
    entry->red = *data;
    ++data;
    entry->green = *data;
    ++data;
    entry->blue = *data;
    ++data;
  }

  return out;
}

int PNGWriteData_PLTE(const struct PNGChunkData_PLTE *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_PLTE(struct PNGChunkData_PLTE *data) {
  free(data->entries);
  free(data);
}

struct PNGChunkData_bKGD *PNGLoadData_bKGD(const uint8_t *data, int data_size) {
  if (data_size != 1)
    return NULL;

  struct PNGChunkData_bKGD *out = malloc(sizeof(struct PNGChunkData_bKGD));
  if (!out)
    return NULL;

  out->index_in_palette = *data;
  return out;
}

int PNGWriteData_bKGD(const struct PNGChunkData_bKGD *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_bKGD(struct PNGChunkData_bKGD *data) {
  free(data);
}

struct PNGChunkData_gAMA *PNGLoadData_gAMA(const uint8_t *data, int data_size) {
  if (data_size != 4)
    return NULL;

  struct PNGChunkData_gAMA *out = malloc(sizeof(struct PNGChunkData_gAMA));
  if (!out)
    return NULL;

  out->gamma = *(const uint32_t *)data;
  if (PNG_IS_LITTLE_ENDIAN)
    InvertBytes(&out->gamma, sizeof(uint32_t));

  return out;
}

int PNGWriteData_gAMA(const struct PNGChunkData_gAMA *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_gAMA(struct PNGChunkData_gAMA *data) {
  free(data);
}

struct PNGChunkData_pHYs *PNGLoadData_pHYs(const uint8_t *data, int data_size) {
  if (data_size != 9)
    return NULL;

  struct PNGChunkData_pHYs *out = malloc(sizeof(struct PNGChunkData_pHYs));
  if (!out)
    return NULL;

  out->x_pixels_per_unit = *(const uint32_t *)data;
  data += sizeof(uint32_t);
  out->y_pixels_per_unit = *(const uint32_t *)data;
  data += sizeof(uint32_t);
  out->unit = *data;
  if (PNG_IS_LITTLE_ENDIAN) {
    InvertBytes(&out->x_pixels_per_unit, sizeof(uint32_t));
    InvertBytes(&out->y_pixels_per_unit, sizeof(uint32_t));
  }

  return out;
}

int PNGWriteData_pHYs(const struct PNGChunkData_pHYs *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_pHYs(struct PNGChunkData_pHYs *data) {
  free(data);
}

struct PNGChunkData_sRGB *PNGLoadData_sRGB(const uint8_t *data, int data_size) {
  if (data_size != 1)
    return NULL;

  struct PNGChunkData_sRGB *out = malloc(sizeof(struct PNGChunkData_sRGB));
  if (!out)
    return NULL;

  out->rendering_intent = *data;

  return out;
}

int PNGWriteData_sRGB(const struct PNGChunkData_sRGB *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_sRGB(struct PNGChunkData_sRGB *data) {
  free(data);
}

struct PNGChunkData_IDAT *PNGLoadData_IDAT(const uint8_t *data, int data_size) {
  struct PNGChunkData_IDAT *out = malloc(sizeof(struct PNGChunkData_IDAT));
  if (!out)
    return NULL;

  out->data = malloc(data_size);
  if (!out->data) {
    free(out);
    return NULL;
  }

  memcpy(out->data, data, data_size);
  out->data_size = data_size;
  return out;
}

int PNGWriteData_IDAT(const struct PNGChunkData_IDAT *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_IDAT(struct PNGChunkData_IDAT *data) {
  free(data->data);
  free(data);
}

struct PNGChunkData_sBIT *PNGLoadData_sBIT(const uint8_t *data, int data_size) {
  if (data_size != 4)
    return NULL;

  struct PNGChunkData_sBIT *out = malloc(sizeof(struct PNGChunkData_sBIT));
  if (!out)
    return NULL;

  memcpy(out->bytes, data, data_size);

  return out;
}

int PNGWriteData_sBIT(const struct PNGChunkData_sBIT *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_sBIT(struct PNGChunkData_sBIT *data) {
  free(data);
}

struct PNGChunkData_UnknownData *PNGLoadData_UnknownData(const uint8_t *data, int data_size) {
  struct PNGChunkData_UnknownData *out = malloc(sizeof(struct PNGChunkData_UnknownData));
  if (!out)
    return NULL;

  out->data = malloc(data_size);
  if (!out->data) {
    free(out);
    return NULL;
  }

  memcpy(out->data, data, data_size);
  out->data_size = data_size;

  return out;
}

int PNGWriteData_UnknownData(const struct PNGChunkData_UnknownData *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_UnknownData(struct PNGChunkData_UnknownData *data) {
  free(data->data);
  free(data);
}

struct PNGChunkData_IEND *PNGLoadData_IEND(const uint8_t *data, int data_size) {
  if (data_size > 0)
    return NULL;

  struct PNGChunkData_IEND *out = malloc(sizeof(struct PNGChunkData_IEND));
  if (!out)
    return NULL;

  return out;
}

int PNGWriteData_IEND(const struct PNGChunkData_IEND *data, void *out) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

void PNGFreeData_IEND(struct PNGChunkData_IEND *data) {
  free(data);
}

void PNGInitRawChunk(struct PNGRawChunk *obj) {
  obj->length = 0;
  obj->type = CHUNK_INVALID;
  obj->raw_data = NULL;
  obj->parsed_data = NULL;
  obj->crc = 0;
}

void PNGFreeRawChunkList(struct PNGRawChunk *node) {
  if (!node)
    return;

  while (node) {
    struct PNGRawChunk *next = node->next;
    free(node->raw_data);
    if (node->parsed_data) {
      struct PNGChunkDataStructFunctions functions = PNGGetChunkDataStructFunctions(node->type);
      assert(functions.free_func);
      functions.free_func(node->parsed_data);
    }
    free(node);
    node = next;
  }
}
