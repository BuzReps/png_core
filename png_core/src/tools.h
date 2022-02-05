#pragma once

#ifndef PNG_IS_LITTLE_ENDIAN
// ASSUMING LITTLE ENDIAN
#define PNG_IS_LITTLE_ENDIAN 1
#endif  // !PNG_ENDIANESS

/* Reverses bytes in buffer */
void InvertBytes(void* buffer, int buffer_size);

