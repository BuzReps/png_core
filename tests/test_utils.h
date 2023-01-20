#pragma once

#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "../png_core/src/tools.h"

namespace test_utils {
/// Create and open unique file in dir
std::pair<std::ofstream, std::filesystem::path> CreateUniqueFile(const std::filesystem::path dir,
                                                                 std::ios_base::openmode mode = std::ios_base::binary);

/// Read file contents as binary data
std::vector<uint8_t> ReadBinaryFile(std::string_view filename);

/// Write binary data to file
void WriteBinaryFile(std::string_view filename, std::vector<uint8_t> const& bytes);

template <typename T>
class VectorWrapper : public std::vector<T> {
  using base_t = std::vector<T>;
  using this_t = VectorWrapper<T>;

public:
  using base_t::base_t;

  template <typename M>
  this_t& Append(const std::vector<M>& vec) {
    std::copy(vec.begin(), vec.end(), std::back_inserter(*this));
    return *this;
  }

  /// Split primitive type into bytes and append to vector
  template <typename N>
  this_t& AppendBytes(N value, bool is_network_order) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
    const bool in_native_order =
        (!is_network_order && PNG_IS_LITTLE_ENDIAN) || (is_network_order && !PNG_IS_LITTLE_ENDIAN);
    if (in_native_order) {
      for (int i = 0; i < sizeof(N); ++i)
        this->push_back(ptr[i]);
    } else
      for (int i = sizeof(N) - 1; i >= 0; --i)
        this->push_back(ptr[i]);
    return *this;
  }
};
}  // namespace test_utils

template <typename T>
bool operator==(const test_utils::VectorWrapper<T>& v1, const std::vector<T>& v2) {
  return static_cast<const std::vector<T>&>(v1) == v2;
}

/// Convenient vector concatenation
template <typename T>
std::vector<T> operator+(const std::vector<T>& v1, const std::vector<T>& v2) {
  auto res = v1;
  std::copy(v2.begin(), v2.end(), std::back_inserter(res));
  return res;
}
