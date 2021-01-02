#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include "request.hpp"
#include "response.hpp"
#include "cache.hpp"
#include <deque>
#include <future>

namespace caching { 
    class Server {
        public:
        Server(const std::string &config_path);

        void AddRequest(std::shared_ptr<Request> request);
        const std::chrono::milliseconds GetTTL() const;
        void Start();
        void Wait();

        private:
        std::mutex m_request_mutex;

        std::deque<std::shared_ptr<Request>> m_requests;
        std::vector<std::future<void>> m_futures;
        std::shared_ptr<Response> HandleRequest(std::shared_ptr<Request> request);
        YAML::Node m_config;
        Cache m_cache;
    };
}