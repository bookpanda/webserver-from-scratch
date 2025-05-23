cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Catch2 CONFIG REQUIRED)


file(GLOB_RECURSE SOURCE_FILES src/*.cpp include/*.hpp)
add_executable(server ${SOURCE_FILES})
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE Threads::Threads ZLIB::ZLIB)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
add_executable(test_string_utils ${TEST_SOURCES} src/utils/string_utils.cpp)
target_include_directories(test_string_utils PRIVATE include)
target_link_libraries(test_string_utils PRIVATE Catch2::Catch2WithMain)