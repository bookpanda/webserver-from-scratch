#include "router/router.hpp"
#include "utils/string_utils.hpp"
#include <format>
#include <string>
#include <iostream>
#include <map>

const std::string HTTP_OK = "HTTP/1.1 200 OK\r\n";
const std::string HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n";
const std::string CONTENT_TEXT = "Content-Type: text/plain\r\n";
const std::string CONTENT_LENGTH = "Content-Length: ";

std::string handleRequest(const std::string &method, const std::string &path, const std::map<std::string, std::string> &headers)
{
    if (method == "GET" && path == "/")
    {
        return HTTP_OK + "\r\n";
    }

    const std::vector<std::string> pathParts = splitString(path, "/");
    if (pathParts.size() > 1 && pathParts[1] == "echo")
    {
        std::string contentLength = CONTENT_LENGTH + std::to_string(pathParts[2].size());
        return std::format("{}{}{}\r\n\r\n{}", HTTP_OK, CONTENT_TEXT, contentLength, pathParts[2]);
    }
    else if (pathParts.size() > 1 && pathParts[1] == "user-agent")
    {
        std::string contentLength = CONTENT_LENGTH + std::to_string(headers.at("User-Agent").size());
        return std::format("{}{}{}\r\n\r\n{}", HTTP_OK, CONTENT_TEXT, contentLength, headers.at("User-Agent"));
    }

    return HTTP_NOT_FOUND + "\r\n";
}