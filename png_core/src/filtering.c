#include "png_core/filtering.h"
#include "png_core/chunk_data.h"
#include "png_core/decoder.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

enum Filter0FuncType {
  NONE = 0,
  SUB = 1,
  UP = 2,
  AVERAGE = 3,
  PAETH = 4
};

static uint8_t FuncNoneFilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return x;
}
static uint8_t FuncNoneDefilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return x;
}

static uint8_t FuncSubFilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return ((int)x - a + 256) % 256;
}
static uint8_t FuncSubDefilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return ((int)x + a) % 256;
}

static uint8_t FuncUpFilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return ((int)x - b + 256) % 256;
}
static uint8_t FuncUpDefilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return ((int)x + b) % 256;
}

static uint8_t FuncAverageFilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  const int sum = (int)floor((a + b) / 2.0);
  return (((int)x + 256) - sum) % 256;
}
static uint8_t FuncAverageDefilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  const int sum = (int)floor((a + b) / 2.0);
  return ((int)x + sum) % 256;
}

static uint8_t Paeth(uint8_t a, uint8_t b, uint8_t c) {
  const int p = (int)a + b - c;
  const int pa = abs(p - a);
  const int pb = abs(p - b);
  const int pc = abs(p - c);
  if (pa <= pb && pa <= pc)
    return a;
  if (pb <= pc)
    return b;
  return c;
}

static uint8_t FuncPaethFilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  // TODO IMPLEMENT
  assert(false && "Not implemented");
  return 0;
}

static uint8_t FuncPaethDefilter(uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  return (x + Paeth(a, b, c)) % 256;
}

typedef uint8_t (*Method0DefilteringFunction)(uint8_t, uint8_t, uint8_t, uint8_t);
typedef Method0DefilteringFunction Method0FilteringFunction;

static Method0DefilteringFunction s_method0_defiltering_functions[5] = {
    FuncNoneDefilter, FuncSubDefilter, FuncUpDefilter, FuncAverageDefilter, FuncPaethDefilter};

static Method0FilteringFunction s_method0_filtering_functions[5] = {FuncNoneFilter, FuncSubFilter, FuncUpFilter,
                                                                    FuncAverageFilter, FuncPaethFilter};

/*
 * @brief Defilter a single byte using compression method 0
 * @param type Filtering function type
 * @param a - corresponding byte in left defiltered pixel
 * @param b - corresponding byte in top defiltered pixel
 * @param c - corresponding byte in top-left defiltered pixel
 */
static inline uint8_t DefilterByteMethod0(uint8_t func_type, uint8_t x, uint8_t a, uint8_t b, uint8_t c) {
  assert(func_type >= 0 && func_type <= 4);
  return s_method0_defiltering_functions[func_type](x, a, b, c);
}

bool PNGDefilterScanlines0(const uint8_t* filtered, int filtered_size, int scanline_width_px, int pixel_size_bytes,
                           uint8_t* defiltered) {
  const int scanline_size_bytes = scanline_width_px * pixel_size_bytes;
  const int scanlines_count = filtered_size / (1 + scanline_size_bytes);

  int restored_idx = 0;
  for (int i = 0; i < scanlines_count; ++i) {
    const uint8_t filter_type = filtered[i * (1 + scanline_size_bytes)];
    for (int j = 0; j < scanline_size_bytes; ++j) {
      const uint8_t x = filtered[i * (1 + scanline_size_bytes) + (j + 1)];
      const uint8_t a = j >= pixel_size_bytes ? defiltered[i * scanline_size_bytes + (j - pixel_size_bytes)] : 0;
      const uint8_t b = i > 0 ? defiltered[(i - 1) * scanline_size_bytes + j] : 0;
      const uint8_t c =
          i > 0 ? j >= pixel_size_bytes ? defiltered[(i - 1) * scanline_size_bytes + (j - pixel_size_bytes)] : 0 : 0;
      defiltered[restored_idx++] = DefilterByteMethod0(filter_type, x, a, b, c);
    }
  }

  return true;
}

PNGDataDefilteringFunction PNGGetDefilteringFunction(uint8_t filtering_method) {
  switch (filtering_method) {
    case PNG_FILTERING_METHOD_0: return PNGDefilterScanlines0;
    default: return NULL;
  }
}

int PNGGetFilteredImageSizeBytes(const struct PNGChunkData_IHDR* header) {
  const int bytes_in_total_image = PNGGetPlainImageSizeBits(header);
  switch (header->filter_method) {
    case PNG_FILTERING_METHOD_0: {
      return bytes_in_total_image + header->height;
    }
    default: break;
  }
  return -1;
}

