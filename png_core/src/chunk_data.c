#include "png_core/chunk_data.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "tools.h"

const uint8_t s_png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

/* 2^31 - 1 */
const int32_t s_png_max_chunk_data_size_bytes = INT32_MAX;

void PNGInitChunkDataStructFunctions(struct PNGChunkDataStructFunctions *obj) {
  obj->alloc_func = NULL;
  obj->init_func = NULL;
  obj->load_func = NULL;
  obj->write_func = NULL;
  obj->free_func = NULL;
}

struct PNGChunkDataStructFunctions PNGGetChunkDataStructFunctions(struct ChunkType type) {
/**
 * Returns function set if type matches
 */
#define PNG_RETURN_FUNCTION_SET(chunk_name)                                              \
  if (type.bytes == CHUNK_##chunk_name.bytes) {                                          \
    struct PNGChunkDataStructFunctions functions;                                        \
    PNGInitChunkDataStructFunctions(&functions);                                         \
    functions.alloc_func = (PNGChunkDataStructAllocateFunc)PNGAllocateData_##chunk_name; \
    functions.init_func = (PNGChunkDataStructInitFunc)PNGInitData_##chunk_name;          \
    functions.load_func = (PNGChunkDataStructLoadFunc)PNGLoadData_##chunk_name;          \
    functions.write_func = (PNGChunkDataStructWriteFunc)PNGWriteData_##chunk_name;       \
    functions.free_func = (PNGChunkDataStructFreeFunc)PNGFreeData_##chunk_name;          \
    return functions;                                                                    \
  }
  PNG_RETURN_FUNCTION_SET(IHDR)
  PNG_RETURN_FUNCTION_SET(sRGB)
  PNG_RETURN_FUNCTION_SET(pHYs)
  PNG_RETURN_FUNCTION_SET(tEXt)
  PNG_RETURN_FUNCTION_SET(PLTE)
  PNG_RETURN_FUNCTION_SET(bKGD)
  PNG_RETURN_FUNCTION_SET(IEND)
  PNG_RETURN_FUNCTION_SET(gAMA)
  PNG_RETURN_FUNCTION_SET(IDAT)
  PNG_RETURN_FUNCTION_SET(sBIT)
#undef PNG_RETURN_FUNCTION_SET

  struct PNGChunkDataStructFunctions functions;
  PNGInitChunkDataStructFunctions(&functions);
  functions.alloc_func = (PNGChunkDataStructAllocateFunc)PNGAllocateData_UnknownData;
  functions.init_func = (PNGChunkDataStructInitFunc)PNGInitData_UnknownData;
  functions.load_func = (PNGChunkDataStructLoadFunc)PNGLoadData_UnknownData;
  functions.write_func = (PNGChunkDataStructWriteFunc)PNGWriteData_UnknownData;
  functions.free_func = (PNGChunkDataStructFreeFunc)PNGFreeData_UnknownData;
  return functions;
}

/**
 * Implements allocating function
 */
#define PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(chunk_type)                                         \
  struct PNGChunkData_##chunk_type *PNGAllocateData_##chunk_type() {                          \
    struct PNGChunkData_##chunk_type *obj = malloc(sizeof(struct PNGChunkData_##chunk_type)); \
    if (obj)                                                                                  \
      PNGInitData_##chunk_type(obj);                                                          \
    return obj;                                                                               \
  }

PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(IHDR)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(sRGB)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(pHYs)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(tEXt)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(PLTE)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(bKGD)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(IEND)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(gAMA)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(IDAT)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(sBIT)
PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE(UnknownData)

#undef PNG_IMPLEMENT_CHUNK_DATA_ALLOCATE

void PNGInitData_IHDR(struct PNGChunkData_IHDR *obj) {
  assert(obj);

  obj->width = 0;
  obj->height = 0;
  obj->bit_depth = 0;
  obj->color_type = 0;
  obj->compression_method = 0;
  obj->filter_method = 0;
  obj->interlace_method = 0;
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
    FlipBytesInBuffer(&out->width, sizeof(int32_t));
    FlipBytesInBuffer(&out->height, sizeof(int32_t));
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

void PNGInitData_tEXt(struct PNGChunkData_tEXt *obj) {
  assert(obj);

  obj->keyword = NULL;
  obj->text = NULL;
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

void PNGInitData_PLTE(struct PNGChunkData_PLTE *obj) {
  assert(obj);

  obj->entries = NULL;
  obj->entries_count = 0;
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

void PNGInitData_bKGD(struct PNGChunkData_bKGD *obj) {
  assert(obj);

  obj->index_in_palette = 0;
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

void PNGInitData_gAMA(struct PNGChunkData_gAMA *obj) {
  assert(obj);

  obj->gamma = 0;
}

struct PNGChunkData_gAMA *PNGLoadData_gAMA(const uint8_t *data, int data_size) {
  if (data_size != 4)
    return NULL;

  struct PNGChunkData_gAMA *out = malloc(sizeof(struct PNGChunkData_gAMA));
  if (!out)
    return NULL;

  out->gamma = *(const uint32_t *)data;
  if (PNG_IS_LITTLE_ENDIAN)
    FlipBytesInBuffer(&out->gamma, sizeof(uint32_t));

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

void PNGInitData_pHYs(struct PNGChunkData_pHYs *obj) {
  assert(obj);

  obj->x_pixels_per_unit = 0;
  obj->y_pixels_per_unit = 0;
  obj->unit = 0;
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
    FlipBytesInBuffer(&out->x_pixels_per_unit, sizeof(uint32_t));
    FlipBytesInBuffer(&out->y_pixels_per_unit, sizeof(uint32_t));
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

void PNGInitData_sRGB(struct PNGChunkData_sRGB *obj) {
  assert(obj);

  obj->rendering_intent = 0;
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

void PNGInitData_IDAT(struct PNGChunkData_IDAT *obj) {
  assert(obj);

  obj->data = NULL;
  obj->data_size = 0;
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

void PNGInitData_sBIT(struct PNGChunkData_sBIT *obj) {
  assert(obj);

  memset(obj->bytes, 0, sizeof(obj->bytes) / sizeof(uint8_t));
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

void PNGInitData_UnknownData(struct PNGChunkData_UnknownData *obj) {
  assert(obj);

  obj->data = NULL;
  obj->data_size = 0;
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

void PNGInitData_IEND(struct PNGChunkData_IEND *obj) {
  assert(obj);

  obj->unused = 0;
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

struct PNGRawChunk *PNGAllocateRawChunk() {
  struct PNGRawChunk *obj = malloc(sizeof(struct PNGRawChunk));
  if (!obj)
    PNGInitRawChunk(obj);
  return obj;
}

void PNGInitRawChunk(struct PNGRawChunk *obj) {
  assert(obj);

  obj->raw_data_size_bytes = 0;
  obj->type = CHUNK_INVALID;
  obj->raw_data = NULL;
  obj->parsed_data = NULL;
  PNGInitChunkDataStructFunctions(&obj->data_functions);
  obj->crc = 0;
  obj->next = NULL;
}

struct PNGRawChunk *PNGLoadRawChunk(const uint8_t *data, int data_size) {
  if (data_size < 12)
    return NULL;

  const uint32_t chunk_data_size = ReadNetworkAndAdvanceInt32(&data, true);
  if (chunk_data_size > s_png_max_chunk_data_size_bytes || 12 + chunk_data_size != data_size)
    return NULL;

  struct PNGRawChunk *chunk = malloc(sizeof(struct PNGRawChunk));
  PNGInitRawChunk(chunk);

  chunk->raw_data_size_bytes = chunk_data_size;
  ReadNetworkAndAdvanceBytesInPlace(&data, chunk->type.byte_array, sizeof(chunk->type.byte_array));
  chunk->raw_data = ReadNetworkAndAdvanceBytes(&data, chunk->raw_data_size_bytes);
  /* Allocation fail */
  if (chunk->raw_data_size_bytes > 0 && !chunk->raw_data) {
    free(chunk);
    return NULL;
  }
  chunk->data_functions = PNGGetChunkDataStructFunctions(chunk->type);
  assert(chunk->data_functions.load_func);
  chunk->parsed_data = chunk->data_functions.load_func(chunk->raw_data, chunk->raw_data_size_bytes);
  /* Allocation fail or definitely invalid data */
  if (!chunk->parsed_data) {
    free(chunk);
    return NULL;
  }
  chunk->crc = ReadNetworkAndAdvanceUInt32(&data, true);

  return chunk;
}

struct PNGRawChunk *PNGLoadRawChunkList(const uint8_t *data, int data_size, bool data_with_png_signature) {
  const uint8_t *data_end = data + data_size;
  if (data_with_png_signature)
    data += sizeof(s_png_signature);

  struct PNGRawChunk dummyHead;
  dummyHead.next = NULL;
  struct PNGRawChunk *last_chunk = &dummyHead;

  while (data < data_end) {
    if (data_end - data < 12)
      return dummyHead.next;

    const uint32_t chunk_length = ReadNetworkAndAdvanceInt32(&data, false);
    if (chunk_length > s_png_max_chunk_data_size_bytes)
      return dummyHead.next;

    const uint8_t *chunk_end = data + 12 + chunk_length;
    if (chunk_end > data_end)
      return dummyHead.next;

    last_chunk->next = PNGLoadRawChunk(data, 12 + chunk_length);
    if (!last_chunk->next)
      return dummyHead.next;

    last_chunk = last_chunk->next;
    data = chunk_end;
  }

  return dummyHead.next;
}

int PNGWriteRawChunk(const struct PNGRawChunk *obj, uint8_t *out) {
  assert(obj);

  if (!out) {
    int data_size = 0;
    if (obj->raw_data)
      data_size = obj->raw_data_size_bytes;
    else if (obj->parsed_data)
      data_size = obj->data_functions.write_func(obj, NULL);

    const int expected_size = sizeof(int32_t) + sizeof(uint32_t) + data_size + sizeof(uint32_t);
    return expected_size;
  }

  WriteNetworkAndAdvanceInt32(&out, obj->raw_data_size_bytes);
  WriteNetworkAndAdvanceBytes(&out, obj->type.byte_array, 4);
  int data_size = obj->raw_data_size_bytes;
  if (obj->raw_data)
    WriteNetworkAndAdvanceBytes(&out, obj->raw_data, obj->raw_data_size_bytes);
  else if (obj->parsed_data) {
    data_size = obj->data_functions.write_func(obj, NULL);
    obj->data_functions.write_func(obj, out);
    out += data_size;
  }
  WriteNetworkAndAdvanceUInt32(&out, obj->crc);

  return sizeof(int32_t) + sizeof(uint32_t) + data_size + sizeof(uint32_t);
}

int PNGWriteRawChunkList(const struct PNGRawChunk *obj, uint8_t *out, bool write_png_signature) {
  if (!out) {
    int total_size = 0;
    if (write_png_signature)
      total_size += sizeof(s_png_signature);
    const struct PNGRawChunk *chunk = obj;
    while (obj) {
      total_size += PNGWriteRawChunk(obj, NULL);
      obj = obj->next;
    }
    return total_size;
  }

  int total_size = 0;
  if (write_png_signature) {
    WriteNetworkAndAdvanceBytes(&out, s_png_signature, sizeof(s_png_signature));
    total_size += sizeof(s_png_signature);
  }
  const struct PNGRawChunk *chunk = obj;
  while (obj) {
    int bytes_written = PNGWriteRawChunk(obj, out);
    total_size += bytes_written;
    out += bytes_written;
    obj = obj->next;
  }
  return total_size;
}

void PNGFreeRawChunk(struct PNGRawChunk *obj) {
  if (!obj)
    return;

  while (obj) {
    struct PNGRawChunk *next = obj->next;
    free(obj->raw_data);
    if (obj->parsed_data) {
      if (obj->data_functions.free_func)
        obj->data_functions.free_func(obj->parsed_data);
      else {
        /* No provided freeing function for complex object => Possible memory leak. */
        assert(false);
        free(obj->parsed_data);
      }
    }
    free(obj);
    obj = next;
  }
}
