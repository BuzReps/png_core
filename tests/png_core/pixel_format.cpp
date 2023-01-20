#include <png_core/pixel_format.h>

#include "../test_utils.h"

class PixelFormatTestSuite : public ::testing::Test {
private:
};

TEST_F(PixelFormatTestSuite, ImageTypeEnumTest) {
  EXPECT_EQ(0, PNG_IMAGE_TYPE_GREYSCALE);
  EXPECT_EQ(2, PNG_IMAGE_TYPE_TRUECOLOR);
  EXPECT_EQ(3, PNG_IMAGE_TYPE_INDEXED);
  EXPECT_EQ(4, PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA);
  EXPECT_EQ(6, PNG_IMAGE_TYPE_TRUECOLORWITHALPHA);
}

TEST_F(PixelFormatTestSuite, ChannelCountTest) {
  EXPECT_EQ(PNGGetChannelCount(PNG_IMAGE_TYPE_GREYSCALE), 1);
  EXPECT_EQ(PNGGetChannelCount(PNG_IMAGE_TYPE_TRUECOLOR), 3);
  EXPECT_EQ(PNGGetChannelCount(PNG_IMAGE_TYPE_INDEXED), 1);
  EXPECT_EQ(PNGGetChannelCount(PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA), 2);
  EXPECT_EQ(PNGGetChannelCount(PNG_IMAGE_TYPE_TRUECOLORWITHALPHA), 4);
}

TEST_F(PixelFormatTestSuite, AllowedBitDepthsTest) {
  static const auto test_bit_depths = [](PNGImageType type, const std::vector<int>& expected_bit_depths) {
    const int allowed_depths_count = PNGGetAllowedBitDepths(type, nullptr);
    ASSERT_EQ(expected_bit_depths.size(), allowed_depths_count);

    uint8_t* allowed_depths = (uint8_t*)malloc(sizeof(uint8_t) * allowed_depths_count);
    PNGGetAllowedBitDepths(type, allowed_depths);
    ASSERT_EQ(expected_bit_depths, std::vector<int>(allowed_depths, allowed_depths + allowed_depths_count));

    free(allowed_depths);
  };

  test_bit_depths(PNG_IMAGE_TYPE_GREYSCALE, {1, 2, 4, 8, 16});
  test_bit_depths(PNG_IMAGE_TYPE_TRUECOLOR, {8, 16});
  test_bit_depths(PNG_IMAGE_TYPE_INDEXED, {1, 2, 4, 8});
  test_bit_depths(PNG_IMAGE_TYPE_GREYSCALEWITHAPLHA, {8, 16});
  test_bit_depths(PNG_IMAGE_TYPE_TRUECOLORWITHALPHA, {8, 16});
}
