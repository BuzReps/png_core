#pragma once

#include <stdint.h>

#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

enum PNGImageType {
  /* Each pixel is a greyscale sample */
  PNG_IMAGE_TYPE_GREYSCALE = 0,
  /* Each pixel is an R,G,B triple */
  PNG_IMAGE_TYPE_TRUECOLOR = 2,
  /* Each pixel is a palette index */
  PNG_IMAGE_TYPE_INDEXED = 3,
  /* Each pixel is a greyscale sample followed by an alpha sample */
  PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA = 4,
  /* Each pixel is an R,G,B triple followed by an alpha sample */
  PNG_IMAGE_TYPE_TRUECOLORWITHALPHA = 6,
};

PNG_CORE_API int PNGGetChannelCount(enum PNGImageType image_type);

/*
 * Get list of allowed bit depths
 * @param[out] out_depths Buffer to write allowed bit depths into. Can be NULL
 * @return Amount of allowed bit depths in ascending order
 */
PNG_CORE_API int PNGGetAllowedBitDepths(enum PNGImageType image_type, uint8_t* out_depths);

#ifdef __cplusplus
}  // extern "C"
#endif
