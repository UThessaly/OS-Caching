#include "request.hpp"
#include "snowflake.hpp"
#include <thread>

namespace caching
{
    Request::Request(const std::string &path, std::size_t size) : m_size(size), m_path(path), m_id(CreateSnowflake())
    {
    }

    const std::string &Request::GetCacheId() const
    {
        return m_path;
    }
    
    std::size_t Request::GetSize()
    {
        return m_size;
    }

    const bool Request::IsCachable() const
    {
        // return m_path.starts_with("GET");
        return true;
    };
} // namespace caching