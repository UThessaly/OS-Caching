#include "cache.hpp"
#include <memory>
#include <optional>
namespace caching
{
    Cache::Cache(std::size_t capacity) : m_capacity(capacity) {}

    std::shared_ptr<Response> Cache::Get(const std::string &key)
    {
        using namespace std::chrono_literals;
        std::lock_guard<std::mutex> guard(m_mutex);
        if (!m_cache.count(key))
        {
            return nullptr;
        }

        auto [expire_time, data] = m_cache.at(key);

        auto now = std::chrono::system_clock::now().time_since_epoch();

        if (now > expire_time)
        {
            m_cache.erase(key);
            return nullptr;
        }

        return data;
    }

    void Cache::Set(const std::string &key, std::shared_ptr<Response> data, std::chrono::milliseconds ttl)
    {
        using namespace std::chrono_literals;

        auto ms = ttl + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        std::lock_guard<std::mutex> guard(m_mutex);
        DeleteOldest();
        m_cache.insert_or_assign(key, std::make_tuple(ms, data));
    }

    void Cache::DeleteOldest()
    {
        using namespace std::chrono_literals;

        std::optional<std::string> key = {};
        std::chrono::milliseconds old_time = 0ms;
        auto now = std::chrono::system_clock::now().time_since_epoch();

        for (auto &[cache_key, val] : m_cache)
        {
            auto &[time, data] = val;
            if (now > time)
                m_cache.erase(cache_key);
            else if (time < old_time)
                key = cache_key;
        }

        if (!key.has_value())
            return;

        m_cache.erase(key.value());
    }

    void Cache::SetCapacity(std::size_t capacity)
    {
        m_capacity = capacity;
    }

} // namespace caching