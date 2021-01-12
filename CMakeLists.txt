cmake_minimum_required(VERSION 3.0.0)
project(caching_server VERSION 0.1.0)

# Using C++ 17
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES 
                    spdlog/1.8.2
                    docopt.cpp/0.6.3
                    yaml-cpp/0.6.3
                GENERATORS    
                    cmake
                    cmake_find_package_multi
                BASIC_SETUP CMAKE_TARGETS)

find_package(Threads REQUIRED)

add_executable(caching_server src/main.cpp src/snowflake.cpp src/response.cpp src/request.cpp src/cache.cpp src/server.cpp)

target_include_directories(caching_server PUBLIC include)

target_link_libraries(caching_server PRIVATE CONAN_PKG::spdlog CONAN_PKG::docopt.cpp CONAN_PKG::yaml-cpp ${CMAKE_THREAD_LIBS_INIT})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
