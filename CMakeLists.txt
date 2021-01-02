cmake_minimum_required(VERSION 3.0.0)
project(caching_server VERSION 0.1.0)

# Using C++ 17
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

set (conan_cmd "conan")
set (conan_arg_install "install")
set (conan_arg_directory "..")

execute_process(COMMAND ${conan_cmd} ${conan_arg_install} ${conan_arg_directory}
                WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                RESULT_VARIABLE conan_result
                OUTPUT_VARIABLE conan_ver
)


if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS KEEP_RPATHS)
else() 
    message(FATAL_ERROR "Conan was not initialized. Please initialize Conan with 'cd build && conan install ..'." "To install Conan, go to https://docs.conan.io/en/latest/installation.html")
endif()

find_package(Threads REQUIRED)

add_executable(caching_server src/main.cpp src/snowflake.cpp src/response.cpp src/request.cpp src/cache.cpp src/server.cpp)

target_include_directories(caching_server PUBLIC include)

target_link_libraries(caching_server PRIVATE CONAN_PKG::spdlog CONAN_PKG::sqlitecpp CONAN_PKG::docopt.cpp CONAN_PKG::yaml-cpp ${CMAKE_THREAD_LIBS_INIT})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
