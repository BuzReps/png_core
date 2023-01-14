#include "png_core/datastream.h"

#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#include "tools.h"

static void* ParseChunkDataStruct(struct ChunkType chunk_type, const uint8_t* data, int data_size) {
  struct PNGChunkDataStructFunctions type_functions = PNGGetChunkDataStructFunctions(chunk_type);
  if (type_functions.load_func) {
    return type_functions.load_func(data, data_size);
  }
  return NULL;
}

struct PNGRawChunk* PNGCreateFromDatastream(void* data, int data_size) {
  static const int min_chunk_size = sizeof(uint32_t) + sizeof(uint32_t) + 0 + sizeof(uint32_t);
  const uint8_t* const bytes = (const uint8_t*)data;
  const uint8_t* const bytes_end = &bytes[data_size];

  struct PNGRawChunk* chunks_list = NULL;
  struct PNGRawChunk* last_chunk = NULL;

  const uint8_t* current_ptr = bytes;
  while (bytes_end - current_ptr >= min_chunk_size) {
    struct PNGRawChunk* new_chunk = malloc(sizeof(struct PNGRawChunk));
    if (!new_chunk)
      break;
    new_chunk->next = NULL;

    new_chunk->length = *(const uint32_t*)(current_ptr);
    current_ptr += sizeof(uint32_t);
    if (PNG_IS_LITTLE_ENDIAN)
      FlipBytesInBuffer(&new_chunk->length, sizeof(uint32_t));

    for (int i = 0; i < 4; ++i) {
      new_chunk->type.byte_array[i] = *current_ptr;
      ++current_ptr;
    }

    new_chunk->raw_data = malloc(new_chunk->length);
    if (!new_chunk->raw_data) {
      break;
    }
    memcpy(new_chunk->raw_data, current_ptr, new_chunk->length);
    new_chunk->parsed_data = ParseChunkDataStruct(new_chunk->type, current_ptr, new_chunk->length);

    current_ptr += new_chunk->length;

    new_chunk->crc = *(const uint32_t*)current_ptr;
    current_ptr += sizeof(uint32_t);
    if (PNG_IS_LITTLE_ENDIAN)
      FlipBytesInBuffer(&new_chunk->crc, sizeof(uint32_t));

    if (!last_chunk) {
      chunks_list = new_chunk;
      last_chunk = chunks_list;
    } else {
      last_chunk->next = new_chunk;
      last_chunk = new_chunk;
    }
  }

  // TODO RETURN ERROR CODE
  if (current_ptr != bytes_end)
    assert(false && "Datastream parsed with errors");

  return chunks_list;
}
