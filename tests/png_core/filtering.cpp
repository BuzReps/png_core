#include <png_core/filtering.h>

#include "../test_utils.h"

class FilteringTestSuite : public ::testing::Test {
protected:
};

TEST_F(FilteringTestSuite, TestFilteringMethodEnum) {
  EXPECT_EQ(0, PNG_FILTERING_METHOD_0);
}

TEST_F(FilteringTestSuite, TestDefilteringGetter) {
  EXPECT_EQ(PNGDefilterScanlines0, PNGGetDefilteringFunction(PNG_FILTERING_METHOD_0));
}

TEST_F(FilteringTestSuite, TestDefilteringMethod0) {
  std::array<std::pair<std::vector<uint8_t>, std::vector<uint8_t>>, 5> scanlines_to_expected;

  scanlines_to_expected[0].first = {0, 1, 2, 3, 4, 5, 6};
  scanlines_to_expected[0].second = {1, 2, 3, 4, 5, 6};

  scanlines_to_expected[1].first = {1, 1, 2, 3, 4, 5, 6};
  scanlines_to_expected[1].second = {1, 3, 6, 10, 15, 21};

  scanlines_to_expected[2].first = {2, 1, 2, 3, 4, 5, 6};
  scanlines_to_expected[2].second = {1, 2, 3, 4, 5, 6};

  scanlines_to_expected[3].first = {3, 1, 2, 3, 4, 5, 6};
  scanlines_to_expected[3].second = {1, 2, 4, 6, 8, 10};

  scanlines_to_expected[4].first = {4, 1, 2, 3, 4, 5, 6};
  scanlines_to_expected[4].second = {1, 3, 6, 10, 15, 21};

  for (auto [scanlines, expected] : scanlines_to_expected) {
    uint8_t defiltered[6];
    EXPECT_TRUE(PNGDefilterScanlines0(scanlines.data(), scanlines.size(), 6, 1, defiltered));
    EXPECT_EQ(expected, std::vector<uint8_t>(std::begin(defiltered), std::end(defiltered)));
  }
}
