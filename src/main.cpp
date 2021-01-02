#include <docopt/docopt.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include "server.hpp"
#include "cache.hpp"

constexpr const char USAGE[] =
R"(Caching Server
    Usage:
      caching_server [options]
      caching_server (-h | --help)
      caching_server --version

    Options:
      -h --help             Show this screen.
      --version             Show version.
      -c --config=<file>    Config File Path [default: ./config.yaml].
      -t --test=<file>      Testing File Path [default: ./test.yaml].
)";

#include <future>
#include <thread>
#include <string>
#include <memory>
#include <any>
#include "server.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

int main(int argc, char const *argv[])
{
    using namespace caching;

    auto args = docopt::docopt(USAGE, {argv + 1, argv + argc}, true, "Caching Server 1.0");

    auto config_path = args["--config"].asString();

    Server server(config_path);

    auto test_path = args["--test"].asString();

    std::ifstream f(test_path); 

    if(!f) {
      spdlog::critical(R"(File "{}" not found)", test_path);
      exit(505);
    }

    auto tests = YAML::Load(f);

    for(auto test : tests["requests"]) {
        auto request_path = std::to_string(test["id"].as<int>());
        auto size = test["size"].as<unsigned long>();
        auto request = std::make_shared<Request>(request_path, size);
        server.AddRequest(request);
    }

    server.Start();

    server.Wait();


    return 0;
}
