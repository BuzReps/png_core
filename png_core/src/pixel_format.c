#include "png_core/pixel_format.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

int PNGGetChannelCount(enum PNGImageType image_type) {
  switch (image_type) {
    case PNG_IMAGE_TYPE_INDEXED:
    case PNG_IMAGE_TYPE_GREYSCALE: return 1;
    case PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA: return 2;
    case PNG_IMAGE_TYPE_TRUECOLOR: return 3;
    case PNG_IMAGE_TYPE_TRUECOLORWITHALPHA: return 4;
    default: assert(false);
  }
  return 0;
}

int PNGGetAllowedBitDepths(enum PNGImageType image_type, uint8_t* out_depths) {
  int amount = 0;
  int* depths = NULL;
  switch (image_type) {
    case PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA:
    case PNG_IMAGE_TYPE_TRUECOLOR:
    case PNG_IMAGE_TYPE_TRUECOLORWITHALPHA: {
      static int d[] = {8, 16};
      amount = sizeof(d) / sizeof(int);
      depths = d;
      break;
    }
    case PNG_IMAGE_TYPE_INDEXED: {
      static int d[] = {1, 2, 4, 8};
      amount = sizeof(d) / sizeof(int);
      depths = d;
      break;
    }
    case PNG_IMAGE_TYPE_GREYSCALE: {
      static int d[] = {1, 2, 4, 8, 16};
      amount = sizeof(d) / sizeof(int);
      depths = d;
      break;
    }
    default: {
      assert(false);
      return 0;
    }
  }

  if (out_depths != NULL) {
    for (int i = 0; i < amount; ++i)
      out_depths[i] = depths[i];
  }

  return amount;
}
