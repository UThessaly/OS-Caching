#pragma once

#include <map>
#include <mutex>

#include "request.hpp"
#include "response.hpp"

#include <chrono>
#include <memory>
#include <tuple>

namespace caching {
class Cache {
 public:
  Cache(std::size_t capacity);

 public:
  std::shared_ptr<Response> Get(const std::string& key);
  void Set(const std::string& key,
           std::shared_ptr<Response>,
           std::chrono::milliseconds ttl);

  void SetCapacity(std::size_t capacity);

 private:
  void DeleteOldest();

  std::map<std::string,
           std::tuple<std::chrono::nanoseconds, std::shared_ptr<Response>>>
      m_cache{};
  std::mutex m_mutex;
  std::size_t m_capacity;
};
}  // namespace caching