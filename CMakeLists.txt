cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# Find all .cpp and .hpp files recursively in these folders and store them in SOURCE_FILES
file(GLOB_RECURSE SOURCE_FILES src/*.cpp include/*.hpp)
add_executable(server ${SOURCE_FILES})
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE Threads::Threads ZLIB::ZLIB)

# Test for string_utils
add_executable(test_string_utils
  tests/string_utils_test.cpp
  src/utils/string_utils.cpp
)
target_include_directories(test_string_utils PRIVATE include)
target_link_libraries(test_string_utils PRIVATE Catch2::Catch2WithMain ZLIB::ZLIB)

# Test for http server
add_executable(test_http_server tests/http_server_test.cpp)
target_include_directories(test_http_server PRIVATE include)
target_link_libraries(test_http_server PRIVATE Catch2::Catch2WithMain)