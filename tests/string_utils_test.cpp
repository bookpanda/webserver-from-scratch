#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "utils/string_utils.hpp"

TEST_CASE("splitString splits by space", "[splitString]")
{
    std::string input = "GET / HTTP/1.1";
    std::vector<std::string> expected = {"GET", "/", "HTTP/1.1"};

    auto result = splitString(input, " ");

    REQUIRE(result == expected);
}

TEST_CASE("splitString splits by CRLF", "[splitString]")
{
    std::string input = "GET / HTTP/1.1\r\nHost: localhost\r\n";
    std::vector<std::string> expected = {"GET / HTTP/1.1", "Host: localhost"};

    auto result = splitString(input, "\r\n");

    REQUIRE(result == expected);
}

TEST_CASE("splitString handles empty input", "[splitString]")
{
    std::string input = "";
    std::vector<std::string> expected = {};

    auto result = splitString(input, " ");

    REQUIRE(result == expected);
}
