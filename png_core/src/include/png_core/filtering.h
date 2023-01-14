#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "chunk_data.h"
#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

enum PNGFilteringMethod {
  PNG_FILTERING_METHOD_0 = 0
};

/*
 * @brief Defilter scanlines
 * @param filtered A sequence of filtered scanlines with preceding filtering function types
 * @param scanline_width_px Amount of pixels in scanline
 * @param pixel_size_bytes Pixel size in bytes (e.g. channels_count * channels_depth)
 * @param[out] defiltered A pre-allocated buffer of size PNGGetDefiltered0Size(...)
 * to write sequence of restored scanlines
 */
PNG_CORE_API bool PNGDefilterScanlines0(const uint8_t* filtered, int filtered_size, int scanline_width_px,
                                        int pixel_size_bytes, uint8_t* defiltered);

typedef bool (*PNGDataDefilteringFunction)(const uint8_t* filtered, int filtered_size, int scanline_width_px,
                                           int pixel_size_bytes, uint8_t* defiltered);

PNG_CORE_API PNGDataDefilteringFunction PNGGetDefilteringFunction(uint8_t filtering_method);

/*
 * @return -1 if error. Otherwise size of a plain image after filtering stage
 */
PNG_CORE_API int PNGGetFilteredImageSizeBytes(const struct PNGChunkData_IHDR* header);

#ifdef __cplusplus
}  // extern "C"
#endif
