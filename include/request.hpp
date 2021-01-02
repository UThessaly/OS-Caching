#pragma once

#include <cinttypes>
#include <string>

namespace caching { 
    class Request { 
        public:
        Request(const std::string &path, const std::size_t size);

        const std::string &GetCacheId() const;
        const bool IsCachable() const;
        std::size_t GetSize();

        private:
        std::int64_t m_id;
        std::size_t m_size;
        std::string m_path;
    };
}