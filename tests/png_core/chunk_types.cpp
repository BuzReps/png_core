#include <png_core/chunk_types.h>

#include "../test_utils.h"

/// Test set for chunk types constants and their helper functions
class ChunkTypesTestSuite : public ::testing::Test {
private:
};

TEST_F(ChunkTypesTestSuite, TestChunkTypeValidness) {
  ChunkType valid_chunk;
  valid_chunk.byte1 = 'i';
  valid_chunk.byte2 = 'H';
  valid_chunk.byte3 = 'D';
  valid_chunk.byte4 = 'R';

  EXPECT_TRUE(IsValidChunkType(valid_chunk));

  // Name should only contain latin letters
  {
    ChunkType invalid_chunk = valid_chunk;
    invalid_chunk.byte3 = 10;
    EXPECT_FALSE(IsValidChunkType(invalid_chunk));
  }
  {
    ChunkType invalid_chunk = valid_chunk;
    invalid_chunk.byte2 = '-';
    EXPECT_FALSE(IsValidChunkType(invalid_chunk));
  }
  // Private bit should be set in all chunks types
  {
    ChunkType invalid_chunk = valid_chunk;
    invalid_chunk.byte3 = 'd';
    EXPECT_FALSE(IsValidChunkType(invalid_chunk));
  }
}

TEST_F(ChunkTypesTestSuite, TestChunkTypeProperties) {
  // 1st byte: lowercase: chunk is ancillary; uppercase: chunk is critical
  {
    ChunkType ancillary;
    ancillary.byte1 = 'i';
    ancillary.byte2 = 'H';
    ancillary.byte3 = 'D';
    ancillary.byte4 = 'R';
    ASSERT_TRUE(IsChunkTypeAncillary(ancillary));
    ancillary.byte1 = 'I';
    ASSERT_FALSE(IsChunkTypeAncillary(ancillary));
  }

  // 2nd byte: lowercase: chunk type is private; uppercase: chunk type is standard
  {
    ChunkType private_;
    private_.byte1 = 'I';
    private_.byte2 = 'h';
    private_.byte3 = 'D';
    private_.byte4 = 'R';
    ASSERT_TRUE(IsChunkTypePrivate(private_));
    private_.byte2 = 'H';
    ASSERT_FALSE(IsChunkTypePrivate(private_));
  }

  // 3nd byte: lowercase: reserved bit is set; uppercase: reserved bit is not set
  {
    ChunkType reserved;
    reserved.byte1 = 'I';
    reserved.byte2 = 'H';
    reserved.byte3 = 'd';
    reserved.byte4 = 'R';
    ASSERT_TRUE(IsChunkTypeReservedBitSet(reserved));
    reserved.byte3 = 'D';
    ASSERT_FALSE(IsChunkTypeReservedBitSet(reserved));
  }

  // 4nd byte: lowercase: chunk is safe to copy; uppercase: not safe to copy
  {
    ChunkType safe_to_copy;
    safe_to_copy.byte1 = 'I';
    safe_to_copy.byte2 = 'H';
    safe_to_copy.byte3 = 'D';
    safe_to_copy.byte4 = 'r';
    ASSERT_TRUE(IsChunkTypeSafeToCopy(safe_to_copy));
    safe_to_copy.byte4 = 'R';
    ASSERT_FALSE(IsChunkTypeSafeToCopy(safe_to_copy));
  }
}
