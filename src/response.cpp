#include "response.hpp"

namespace caching {
Response::Response(const std::string& data) : m_data(data) {}

const std::string& Response::GetData() const {
  return m_data;
}
}  // namespace caching