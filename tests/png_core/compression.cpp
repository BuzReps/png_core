#include <png_core/compression.h>

#include "../test_utils.h"

/// Test set for compression functions
class CompressionTestSuite : public ::testing::Test {
protected:
  static inline std::vector<PNGDataDecompressionFunction> decompression_functions_ = {PNGDataDecompress0};
};

TEST_F(CompressionTestSuite, TestCompressionMethodsGetters) {
  EXPECT_EQ(PNGDataDecompress0, PNGGetDataDecompressionFunction(PNGCompressionMethod::PNG_COMPRESSION_METHOD_0));
}

TEST_F(CompressionTestSuite, TestDecompressionFunctions) {
  // clang-format off
  static const std::string source_string =
    "\n"
    "The missile knows where it is at all times.\n"
    "It knows this because it knows where it isn't.\n"
    "By subtracting where it is from where it isn't, or where it isn't from where it is (whichever is greater), it obtains a difference, or deviation.\n"
    "The guidance subsystem uses deviations to generate corrective commands to drive the missile from a position where it is to a position where it isn't, and arriving at a position where it wasn't, it now is.\n"
    "Consequently, the position where it is, is now the position that it wasn't, and it follows that the position that it was, is now the position that it isn't.\n"
    "In the event that the position that it is in is not the position that it wasn't, the system has acquired a variation, the variation being the difference between where the missile is, and where it wasn't.\n"
    "If variation is considered to be a significant factor, it too may be corrected by the GEA.\n"
    "However, the missile must also know where it was.\n"
    "The missile guidance computer scenario works as follows.\n"
    "Because a variation has modified some of the information the missile has obtained, it is not sure just where it is.\n"
    "However, it is sure where it isn't, within reason, and it knows where it was.\n"
    "It now subtracts where it should be from where it wasn't, or vice-versa, and by differentiating this from the algebraic sum of where it shouldn't be, and where it was, it is able to obtain the deviation and its variation, which is called error.\n";
  // clang-format on

  static const std::vector<uint8_t> source_compressed_data = {
      0x78, 0x5e, 0x7d, 0x54, 0xcb, 0x72, 0xdb, 0x30, 0x0c, 0xbc, 0xeb, 0x2b, 0x70, 0x6b, 0x3b, 0x93, 0xfa, 0x1f, 0xda,
      0x4e, 0xa7, 0xcd, 0xbd, 0x3f, 0x40, 0x49, 0x90, 0x85, 0x46, 0x22, 0x13, 0x82, 0xb2, 0xc6, 0x7f, 0xdf, 0x05, 0x68,
      0xd9, 0x4a, 0x9c, 0xf4, 0x66, 0x8b, 0x78, 0xec, 0x2e, 0x16, 0x68, 0xfe, 0x8c, 0x4c, 0xb3, 0xa8, 0xca, 0xc4, 0xf4,
      0x14, 0xd3, 0xaa, 0xb4, 0x8e, 0x9c, 0x99, 0xa4, 0x90, 0x28, 0x85, 0x42, 0x61, 0x9a, 0xa8, 0xc8, 0xcc, 0x7a, 0x68,
      0x1e, 0xcb, 0x25, 0xa4, 0x8c, 0x78, 0x6b, 0xb9, 0x0b, 0x8b, 0x7a, 0xe4, 0x5d, 0x62, 0xfc, 0x54, 0x0e, 0xcd, 0xf7,
      0x33, 0xe9, 0xd2, 0x96, 0x1c, 0xba, 0x22, 0xf1, 0xf8, 0xaa, 0xee, 0x90, 0xd3, 0xbc, 0xff, 0x80, 0xf8, 0x07, 0x4a,
      0xf9, 0xcd, 0x27, 0x7a, 0x1b, 0x46, 0x9f, 0xd7, 0x51, 0xba, 0x91, 0x4f, 0x9c, 0x0d, 0xdd, 0x31, 0x73, 0x28, 0x9c,
      0xbf, 0x3c, 0x18, 0x86, 0xd4, 0x96, 0x20, 0x11, 0x90, 0xa9, 0x97, 0x61, 0x00, 0x85, 0xd8, 0xb1, 0xd7, 0xec, 0xf9,
      0x24, 0xa1, 0x48, 0x8a, 0x87, 0xc6, 0xc8, 0x1e, 0x17, 0xe9, 0x03, 0xde, 0x0c, 0x9b, 0x9e, 0xb5, 0xf0, 0x4c, 0x60,
      0xa1, 0xb7, 0x30, 0xa5, 0x92, 0xe8, 0xc8, 0x91, 0x33, 0x8a, 0x53, 0x97, 0x72, 0x66, 0x10, 0x38, 0xd9, 0xcf, 0x79,
      0x0e, 0xb1, 0xf7, 0xf7, 0x3e, 0xdb, 0x97, 0xb2, 0x13, 0xcf, 0xb1, 0x06, 0x7a, 0x4e, 0x2a, 0xd6, 0xec, 0x15, 0x5d,
      0x24, 0xbc, 0xff, 0xe2, 0xbc, 0x51, 0x93, 0x42, 0x46, 0x41, 0x53, 0xc9, 0x14, 0x7f, 0xa7, 0xc8, 0x1a, 0xd4, 0x63,
      0x41, 0x14, 0x43, 0x02, 0xf9, 0x43, 0xf3, 0x23, 0x45, 0xe5, 0x97, 0x85, 0x63, 0x99, 0xce, 0x0f, 0x0e, 0xe5, 0xbd,
      0xde, 0x48, 0x51, 0x4f, 0x79, 0x15, 0x50, 0x46, 0xf4, 0x41, 0xad, 0xad, 0xac, 0x41, 0xc0, 0xdf, 0x21, 0x4d, 0x93,
      0x59, 0xc0, 0x9f, 0x3f, 0x4a, 0xf8, 0x7f, 0x45, 0x31, 0x9c, 0xb0, 0x4a, 0xf4, 0x7c, 0x4c, 0x2a, 0x5a, 0xa5, 0x8f,
      0xca, 0x01, 0x9b, 0xc4, 0x8a, 0xf0, 0xe3, 0x86, 0x4e, 0xdc, 0xaa, 0x5d, 0xc6, 0x35, 0x06, 0x4c, 0xb9, 0x7b, 0x59,
      0x24, 0x33, 0x84, 0xa3, 0x53, 0xc8, 0x75, 0xbe, 0x35, 0xe8, 0xfa, 0x17, 0x06, 0x35, 0x41, 0x2d, 0xf1, 0xe6, 0x08,
      0x7c, 0x2c, 0x2b, 0xf3, 0x36, 0x9e, 0xfd, 0x00, 0x45, 0xeb, 0x28, 0xae, 0xfe, 0xaf, 0xe2, 0x80, 0xcb, 0x70, 0xeb,
      0x61, 0x58, 0x3b, 0x08, 0x2f, 0x3d, 0x1c, 0xd6, 0x9b, 0x13, 0x5a, 0x06, 0x06, 0x95, 0x63, 0x94, 0x41, 0xba, 0x00,
      0xb6, 0x03, 0xfc, 0x9e, 0xb2, 0x8f, 0xaa, 0xa4, 0x44, 0x73, 0x38, 0x5b, 0xcc, 0xc5, 0x47, 0xc8, 0x69, 0xcf, 0x8e,
      0xe9, 0xd7, 0xcf, 0x6f, 0x87, 0xe6, 0x77, 0x5a, 0xcd, 0xcc, 0x15, 0xf9, 0xb6, 0x86, 0xf3, 0xa2, 0xb6, 0x76, 0x9a,
      0x7c, 0xaf, 0x6e, 0x46, 0x02, 0x9e, 0x6a, 0xe1, 0x2d, 0xf0, 0x6a, 0x65, 0x38, 0xf3, 0x79, 0xc1, 0x26, 0x90, 0x76,
      0x1c, 0x21, 0x47, 0xa2, 0x35, 0xe5, 0x27, 0xa8, 0xa4, 0xdb, 0x48, 0xb1, 0x8d, 0x97, 0x75, 0xdd, 0x09, 0x46, 0xa6,
      0xe4, 0x9c, 0xa0, 0x8e, 0x00, 0x97, 0xa6, 0x99, 0x29, 0x0d, 0x0e, 0x4e, 0xe2, 0x90, 0xf2, 0xec, 0x5b, 0xe3, 0xff,
      0xb7, 0x8e, 0x96, 0x50, 0x17, 0x8d, 0x7b, 0x67, 0xe8, 0xe6, 0x2a, 0xd8, 0x25, 0x9c, 0x8c, 0xbf, 0x86, 0x7b, 0x77,
      0x3d, 0x76, 0xf4, 0x6a, 0xa4, 0x47, 0xed, 0x02, 0x7c, 0xb0, 0xab, 0xe0, 0x9a, 0x44, 0xc2, 0x2a, 0x6b, 0x8a, 0x75,
      0x02, 0xf7, 0x17, 0xc5, 0xa9, 0x3f, 0x56, 0xef, 0x6f, 0x37, 0x65, 0x77, 0xa9, 0x74, 0x4c, 0xcb, 0x04, 0x65, 0xf9,
      0xcd, 0xc5, 0xd8, 0xec, 0x8d, 0x2b, 0x70, 0x92, 0x8e, 0xbf, 0x42, 0x6a, 0x0d, 0xb5, 0x07, 0xa6, 0xb0, 0xb9, 0xa2,
      0x98, 0x7d, 0xdc, 0x29, 0x60, 0xe3, 0x6b, 0x6c, 0x94, 0xc3, 0x74, 0xe4, 0x36, 0x07, 0xe9, 0x40, 0x6e, 0x36, 0x5d,
      0xae, 0xc0, 0x6b, 0x37, 0x80, 0x47, 0xc3, 0x7b, 0xcb, 0x6c, 0xb2, 0x84, 0x16, 0x17, 0x15, 0xfe, 0xa8, 0x72, 0x55,
      0x1b, 0x6e, 0x97, 0xc8, 0x57, 0x5e, 0x8a, 0xee, 0xbd, 0xeb, 0x97, 0xcd, 0xed, 0x85, 0x8b, 0x8b, 0x71, 0x70, 0xce,
      0x29, 0x1f, 0x9a, 0x7f, 0x17, 0x3f, 0x04, 0x1c};

  for (const auto decompression_function : decompression_functions_) {
    uint8_t* decompressed_data =
        PNGDataDecompress0(source_compressed_data.data(), source_compressed_data.size(), source_string.size());
    ASSERT_TRUE(decompressed_data);

    const auto decompressed_string = std::string((char*)decompressed_data);
    EXPECT_EQ(source_string, decompressed_string);

    PNGFreeCompressionData(decompressed_data);
  }
}