#pragma once

// Expands to 1 if target architecture is little endian (e.g. x86) or to 0 if big endian
// This macro is used to reduce runtime code
// Define this macro in compile arguments if cannot determine automatically
#ifndef PNG_IS_LITTLE_ENDIAN

#ifdef __GNUC__  // compiler type
#define PNG_IS_LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

#elif defined(_MSC_VER)  // compiler type
#error TODO IMPLEMENT FOR MSVC

#else  // compiler type
#error Unknown compiler

#endif  // compiler type

#endif  // PNG_IS_LITTLE_ENDIAN

/**
 * Flip bytes in buffer
 */
void FlipBytesInBuffer(void* buffer, int buffer_size);
