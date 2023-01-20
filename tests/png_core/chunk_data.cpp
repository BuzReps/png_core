#include <png_core/chunk_data.h>

#include "../test_utils.h"

/// Test set for chunk data structures from `png_core/chunk_data.h`
class ChunkDataTestSuite : public ::testing::Test {
protected:
  /// Get list of valid PNG images for tests
  static std::vector<std::string> GetAllImages() {
    return std::vector<std::string>{"tests/res/images/image_1.png"};
  }

  /// Get directory for temp files
  static std::filesystem::path GetTmpDir() {
    return std::filesystem::temp_directory_path();
  }

protected:
  /// PNG datastream signature
  static inline const uint8_t expected_png_signature_[8] = {137, 80, 78, 71, 13, 10, 26, 10};

  /// Chunk data field mad size bytes
  static inline const uint32_t expected_max_chunk_size_ = [] {
    return std::pow((uint32_t)2, 31) - 1;
  }();
};

/// Ensure that png_core provides a valid PNG datastream signature constant
TEST_F(ChunkDataTestSuite, ValidatePNGSignature) {
  for (int i = 0; i < 8; ++i)
    EXPECT_EQ(expected_png_signature_[i], s_png_signature[i]);
}

/// Ensure that png_core provides a valid max chunk data size constant
TEST_F(ChunkDataTestSuite, ValidateMaxChunkDataSize) {
  EXPECT_EQ(expected_max_chunk_size_, (uint32_t)s_png_max_chunk_data_size_bytes);
}

TEST_F(ChunkDataTestSuite, TestChunkData_IHDR) {
  PNGChunkData_IHDR chunk;
  chunk.width = 1920;
  chunk.height = 1080;
  chunk.bit_depth = 16;
  chunk.color_type = 1;
  chunk.compression_method = 2;
  chunk.filter_method = 3;
  chunk.interlace_method = 4;

  const auto expected_size = PNGWriteData_IHDR(&chunk, nullptr);
  EXPECT_EQ(expected_size, 13);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_IHDR(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data;
  expected_data.AppendBytes(chunk.width, true);
  expected_data.AppendBytes(chunk.height, true);
  expected_data.AppendBytes(chunk.bit_depth, true);
  expected_data.AppendBytes(chunk.color_type, true);
  expected_data.AppendBytes(chunk.compression_method, true);
  expected_data.AppendBytes(chunk.filter_method, true);
  expected_data.AppendBytes(chunk.interlace_method, true);
  ASSERT_EQ(written_data, expected_data);

  struct PNGChunkData_IHDR* loaded_chunk = PNGLoadData_IHDR(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_IHDR(&chunk, loaded_chunk));

  PNGFreeData_IHDR(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_tEXt) {
  const char* keyword = "Description";
  const char* text = "This is a test tEXt chunk";

  PNGChunkData_tEXt chunk;
  chunk.keyword = const_cast<char*>(keyword);
  chunk.text = const_cast<char*>(text);

  const auto expected_size = PNGWriteData_tEXt(&chunk, nullptr);
  EXPECT_EQ(expected_size, strlen(keyword) + 1 + strlen(text));

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_tEXt(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data = {'D', 'e', 's', 'c', 'r', 'i', 'p', 't', 'i', 'o', 'n', '\0', 'T',
                                                      'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e', 's',  't',
                                                      ' ', 't', 'E', 'X', 't', ' ', 'c', 'h', 'u', 'n', 'k'};
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_tEXt* loaded_chunk = PNGLoadData_tEXt(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_tEXt(&chunk, loaded_chunk));

  PNGFreeData_tEXt(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_PLTE) {
  PaletteDataEntry palette[4];
  palette[0] = {1, 2, 3};
  palette[1] = {4, 5, 6};
  palette[2] = {7, 8, 9};
  palette[3] = {10, 11, 12};

  PNGChunkData_PLTE chunk;
  chunk.entries_count = 4;
  chunk.entries = palette;

  const auto expected_size = PNGWriteData_PLTE(&chunk, nullptr);
  EXPECT_EQ(expected_size, chunk.entries_count * 3);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_PLTE(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_PLTE* loaded_chunk = PNGLoadData_PLTE(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_PLTE(&chunk, loaded_chunk));

  PNGFreeData_PLTE(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_bKGD) {
  PNGChunkData_bKGD chunk;
  chunk.index_in_palette = 47;

  const auto expected_size = PNGWriteData_bKGD(&chunk, nullptr);
  EXPECT_EQ(expected_size, 1);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_bKGD(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data = {47};
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_bKGD* loaded_chunk = PNGLoadData_bKGD(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_bKGD(&chunk, loaded_chunk));

  PNGFreeData_bKGD(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_gAMA) {
  PNGChunkData_gAMA chunk;
  chunk.gamma = 45455;

  const auto expected_size = PNGWriteData_gAMA(&chunk, nullptr);
  EXPECT_EQ(expected_size, 4);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_gAMA(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data;
  expected_data.AppendBytes(chunk.gamma, true);
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_gAMA* loaded_chunk = PNGLoadData_gAMA(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_gAMA(&chunk, loaded_chunk));

  PNGFreeData_gAMA(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_pHYs) {
  PNGChunkData_pHYs chunk;
  chunk.x_pixels_per_unit = 1920;
  chunk.y_pixels_per_unit = 1080;
  chunk.unit = 1;

  const auto expected_size = PNGWriteData_pHYs(&chunk, nullptr);
  EXPECT_EQ(expected_size, 9);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_pHYs(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data;
  expected_data.AppendBytes(chunk.x_pixels_per_unit, true);
  expected_data.AppendBytes(chunk.y_pixels_per_unit, true);
  expected_data.push_back(chunk.unit);
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_pHYs* loaded_chunk = PNGLoadData_pHYs(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_pHYs(&chunk, loaded_chunk));

  PNGFreeData_pHYs(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_sRGB) {
  PNGChunkData_sRGB chunk;
  chunk.rendering_intent = PNGRenderingIntent::PNG_RENDERING_INTENT_SATURATION;

  const auto expected_size = PNGWriteData_sRGB(&chunk, nullptr);
  EXPECT_EQ(expected_size, 1);

  std::vector<uint8_t> written_data(expected_size, 1);
  const auto written_bytes = PNGWriteData_sRGB(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data = {2};
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_sRGB* loaded_chunk = PNGLoadData_sRGB(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_sRGB(&chunk, loaded_chunk));

  PNGFreeData_sRGB(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_IDAT) {
  uint8_t data[543];
  for (int i = 0; i < sizeof(data); ++i)
    data[i] = i % 256;

  PNGChunkData_IDAT chunk;
  chunk.data_size = sizeof(data);
  chunk.data = data;

  const auto expected_size = PNGWriteData_IDAT(&chunk, nullptr);
  EXPECT_EQ(expected_size, sizeof(data));

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_IDAT(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data(std::begin(data), std::end(data));
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_IDAT* loaded_chunk = PNGLoadData_IDAT(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_IDAT(&chunk, loaded_chunk));

  PNGFreeData_IDAT(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_sBIT) {
  PNGChunkData_sBIT chunk;
  chunk.bytes[0] = 42;
  chunk.bytes[1] = 11;
  chunk.bytes[2] = 7;
  chunk.bytes[3] = 8;

  const auto expected_size = PNGWriteData_sBIT(&chunk, nullptr);
  EXPECT_EQ(expected_size, 4);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_sBIT(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data = {42, 11, 7, 8};
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_sBIT* loaded_chunk = PNGLoadData_sBIT(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_sBIT(&chunk, loaded_chunk));

  PNGFreeData_sBIT(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_IEND) {
  PNGChunkData_IEND chunk;

  const auto expected_size = PNGWriteData_IEND(&chunk, nullptr);
  EXPECT_EQ(expected_size, 0);

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_IEND(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  uint8_t expected_data[42];
  struct PNGChunkData_IEND* loaded_chunk = PNGLoadData_IEND(expected_data, sizeof(expected_data));
  ASSERT_FALSE(loaded_chunk);
  loaded_chunk = PNGLoadData_IEND(expected_data, 0);
  ASSERT_TRUE(PNGEqualData_IEND(&chunk, loaded_chunk));

  PNGFreeData_IEND(loaded_chunk);
}

TEST_F(ChunkDataTestSuite, TestChunkData_UnknownData) {
  uint8_t data[543];
  for (int i = 0; i < sizeof(data); ++i)
    data[i] = i % 256;

  PNGChunkData_UnknownData chunk;
  chunk.data_size = sizeof(data);
  chunk.data = data;

  const auto expected_size = PNGWriteData_UnknownData(&chunk, nullptr);
  EXPECT_EQ(expected_size, sizeof(data));

  std::vector<uint8_t> written_data(expected_size, 0);
  const auto written_bytes = PNGWriteData_UnknownData(&chunk, written_data.data());
  EXPECT_EQ(expected_size, written_bytes);

  test_utils::VectorWrapper<uint8_t> expected_data(std::begin(data), std::end(data));
  EXPECT_EQ(written_data, expected_data);

  struct PNGChunkData_UnknownData* loaded_chunk = PNGLoadData_UnknownData(expected_data.data(), expected_data.size());
  ASSERT_TRUE(loaded_chunk);
  ASSERT_TRUE(PNGEqualData_UnknownData(&chunk, loaded_chunk));

  PNGFreeData_UnknownData(loaded_chunk);
}

/// 1. Loads image as binary data
/// 2. Parses binary data into list of chunks
/// 3. Writes list of chunks as binary data
/// 4. Assumes loaded and written data are equal
TEST_F(ChunkDataTestSuite, LoadAndWritePNGFileTest) {
  for (const auto& path : GetAllImages()) {
    const auto original_data = test_utils::ReadBinaryFile(path);
    const PNGRawChunk* chunk_list = PNGLoadRawChunkList(original_data.data(), original_data.size(), true);
    ASSERT_TRUE(chunk_list);

    const auto bytes = PNGWriteRawChunkList(chunk_list, nullptr, true);
    ASSERT_EQ(original_data.size(), bytes);
    std::vector<uint8_t> written_data(bytes, 0);
    PNGWriteRawChunkList(chunk_list, written_data.data(), true);
    ASSERT_EQ(original_data, written_data);
  }
}
