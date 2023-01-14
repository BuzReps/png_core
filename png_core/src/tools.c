#include "tools.h"

#include <assert.h>
#include <stdint.h>

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
