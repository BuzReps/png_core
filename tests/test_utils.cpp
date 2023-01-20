#include "test_utils.h"

namespace test_utils {
std::pair<std::ofstream, std::filesystem::path> CreateUniqueFile(const std::filesystem::path dir,
                                                                 std::ios_base::openmode mode) {
  size_t i = 0;
  while (true) {
    ++i;
    try {
      const auto path = dir / (std::string("tmp_") + std::to_string(i));
      std::ofstream out(path, mode);
      return std::make_pair(std::move(out), path);
    } catch (std::exception& e) {
    }
  }
}

std::vector<uint8_t> ReadBinaryFile(std::string_view filename) {
  std::ifstream in(filename.data(), std::ios::binary);
  const auto bytes = std::vector<uint8_t>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
  return bytes;
}

void WriteBinaryFile(std::string_view filename, std::vector<uint8_t> const& bytes) {
  std::ofstream out(filename.data(), std::ios::binary);
  std::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(out));
}
}  // namespace test_utils
