#include "router/router.hpp"
#include "utils/string_utils.hpp"
#include <format>
#include <string>
#include <iostream>
#include <map>

std::string handleRequest(const std::string &method, const std::string &path, const std::map<std::string, std::string> &headers)
{
    if (method == "GET" && path == "/")
    {
        return "HTTP/1.1 200 OK\r\n\r\n";
    }

    const std::vector<std::string> pathParts = splitString(path, "/");
    if (pathParts.size() > 1 && pathParts[1] == "echo")
    {
        int contentLength = pathParts[2].size();
        return std::format("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {}\r\n\r\n{}", contentLength, pathParts[2]);
    }
    else if (pathParts.size() > 1 && pathParts[1] == "user-agent")
    {
        int contentLength = headers.at("User-Agent").size();
        return std::format("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: {}\r\n\r\n{}", contentLength, headers.at("User-Agent"));
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"message\": \"Hello, JSON!\"}";
    }

    return "HTTP/1.1 404 Not Found\r\n\r\n";
}