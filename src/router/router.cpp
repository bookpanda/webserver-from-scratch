#include "router/router.hpp"
#include "utils/string_utils.hpp"

std::string handleRequest(const std::string &method, const std::string &path)
{
    if (method == "GET" && path == "/")
    {
        return "HTTP/1.1 200 OK\r\n\r\n";
    }

    const std::vector<std::string> pathParts = splitString(path, "/");
    if (pathParts.size() > 1 && pathParts[1] == "echo")
    {
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 3\r\n\r\n" + pathParts[2];
    }

    return "HTTP/1.1 404 Not Found\r\n\r\n";
}