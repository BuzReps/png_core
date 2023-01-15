#include "tools.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void FlipBytesInBuffer(void* buffer, int buffer_size) {
  assert(buffer);
  assert(buffer_size >= 0);

  uint8_t* bytes = (uint8_t*)buffer;
  for (int i = 0; i < buffer_size / 2; ++i) {
    const uint8_t temp = bytes[i];
    bytes[i] = bytes[buffer_size - i - 1];
    bytes[buffer_size - i - 1] = temp;
  }
}

#define PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(type, name)                      \
  type ReadNetworkAndAdvance##name(const uint8_t** ptr_to_buffer_ptr, bool advance) { \
    type res = *(const type*)(*ptr_to_buffer_ptr);                                    \
    if (PNG_IS_LITTLE_ENDIAN)                                                         \
      FlipBytesInBuffer(&res, sizeof(type));                                          \
    if (advance)                                                                      \
      *ptr_to_buffer_ptr += sizeof(type);                                             \
    return res;                                                                       \
  }                                                                                   \
  void WriteNetworkAndAdvance##name(uint8_t** buffer, type value) {                   \
    if (PNG_IS_LITTLE_ENDIAN)                                                         \
      FlipBytesInBuffer(&value, sizeof(type));                                        \
    memcpy(*buffer, &value, sizeof(type));                                            \
    *buffer += sizeof(type);                                                          \
  }

PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(uint8_t, Byte);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(int8_t, Int8);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(int16_t, Int16);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(uint16_t, UInt16);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(int32_t, Int32);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(uint32_t, UInt32);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(int64_t, Int64);
PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE(uint64_t, UInt64);
#undef PNG_IMPLEMENT_READ_WRITE_NETWORK_AND_ADVANCE

uint8_t* ReadNetworkAndAdvanceBytes(const uint8_t** buffer, int size_bytes) {
  uint8_t* dst = malloc(size_bytes);
  if (!dst)
    return NULL;
  memcpy(dst, (const void*)(*buffer), size_bytes);
  *buffer += size_bytes;
  return dst;
}

void ReadNetworkAndAdvanceBytesInPlace(const uint8_t** buffer, uint8_t* dst, int size_bytes) {
  memcpy(dst, (const void*)(*buffer), size_bytes);
  *buffer += size_bytes;
}

void WriteNetworkAndAdvanceBytes(uint8_t** buffer, const uint8_t* src, int size_bytes) {
  memcpy(*buffer, src, size_bytes);
  *buffer += size_bytes;
}
