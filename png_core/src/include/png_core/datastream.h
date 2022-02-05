#pragma once

#include "png_core.h"

#include "chunk_data.h"

struct PNGRawChunk* PNGCreateFromDatastream(void* data, int data_size);
