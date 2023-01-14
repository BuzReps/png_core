#include <gtest/gtest.h>
#include <png_core/chunk_data.h>

class ChunkDataTestSuite : public ::testing::Test {
protected:
  /// PNG datastream signature
  const uint8_t expected_png_signature_[8] = {137, 80, 78, 71, 13, 10, 26, 10};
};

/// Ensure that png_core provides a valid PNG datastream signature
TEST_F(ChunkDataTestSuite, TestPNGSignature) {
  for (int i = 0; i < 8; ++i)
    EXPECT_EQ(expected_png_signature_[i], s_png_signature[i]);
}
