#pragma once

#include "chunk_data.h"
#include "png_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PNGRawChunk* PNGCreateFromDatastream(void* data, int data_size);

#ifdef __cplusplus
}  // extern "C"
#endif
