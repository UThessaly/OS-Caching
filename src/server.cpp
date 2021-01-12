#include "server.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>

namespace caching
{
    Server::Server(const std::string &config_path) : m_cache(Cache(0))
    {
        if (auto fs = std::ifstream(config_path); fs)
        {
            m_config = YAML::Load(fs);
        }
        else
        {
            spdlog::critical(R"(Error: file '{}' not found)", config_path);
            exit(505);
        }

        m_cache.SetCapacity(m_config["settings"]["capacity"].as<std::size_t>());
    }

    std::shared_ptr<Response> Server::HandleRequest(std::shared_ptr<Request> request)
    {
        auto size = request->GetSize();
        
        using namespace std::chrono_literals;

                if (auto response = m_cache.Get(request->GetCacheId()); response != nullptr)
        {
            spdlog::info("{}: Returned from Cache", request->GetCacheId());
            return response;
        }

        spdlog::warn("{}: Calculating Response ({}, {})", request->GetCacheId(), size, size % 10);
        std::chrono::seconds sleep_time((size % 10));
        std::this_thread::sleep_for(sleep_time);
        spdlog::warn("{}: Calculated Response", request->GetCacheId());
        auto response = std::make_shared<Response>(request->GetCacheId());
        m_cache.Set(request->GetCacheId(), response, GetTTL());
        return response;
    }

    const std::chrono::milliseconds Server::GetTTL() const
    {
        auto time = m_config["settings"]["ttl"].as<long>();

        std::chrono::milliseconds ttl(time);

        return ttl + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }

    void Server::AddRequest(std::shared_ptr<Request> request)
    {
        std::lock_guard<std::mutex> guard(m_request_mutex);
        m_requests.push_back(request);
    }

    void Server::Start() {
        auto threads = m_config["settings"]["threads"].as<int>();

        for(int thread = 0; thread < threads; thread++) {
            m_futures.push_back(std::async(std::launch::async, [&]() {
                while(true) {
                    std::shared_ptr<Request> request;
                    {
                        std::lock_guard guard(m_request_mutex);
                        if(m_requests.size() == 0) {
                            return;
                        }
                        request = m_requests[0];
                        m_requests.pop_front();
                    }

                    HandleRequest(request);
                }
            }));
        }
    }

    void Server::Wait() {
        for(auto& future : m_futures) {
            future.wait();
        }
    }
} // namespace caching