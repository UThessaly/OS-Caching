#pragma once
#include <string>
#include <chrono>

namespace caching { 
    class Response { 
        public:
        Response(const std::string &data);

        const std::string& GetData() const;

        private:
        std::string m_data;
    };
}