#pragma once

#include <stdbool.h>
#include <stdint.h>

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

/**
 * Read number from network-ordered buffer and increment pointer
 * Write number to network-ordered buffer and increment pointer
 */
#define PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(type, name)            \
  type ReadNetworkAndAdvance##name(const uint8_t** buffer, bool advance); \
  void WriteNetworkAndAdvance##name(uint8_t** buffer, type value);

PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(uint8_t, Byte);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(int8_t, Int8);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(int16_t, Int16);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(uint16_t, UInt16);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(int32_t, Int32);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(uint32_t, UInt32);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(int64_t, Int64);
PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE(uint64_t, UInt64);
#undef PNG_DECLARE_READ_WRITE_NETWORK_AND_ADVANCE

uint8_t* ReadNetworkAndAdvanceBytes(const uint8_t** buffer, int size_bytes);
void ReadNetworkAndAdvanceBytesInPlace(const uint8_t** buffer, uint8_t* dst, int size_bytes);
void WriteNetworkAndAdvanceBytes(uint8_t** buffer, const uint8_t* src, int size_bytes);
