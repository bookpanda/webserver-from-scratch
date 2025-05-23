#include "router/router.hpp"

std::string handleRequest(const std::string &method, const std::string &path)
{
    if (method == "GET" && path == "/")
    {
        return "HTTP/1.1 200 OK\r\n\r\n";
    }

    return "HTTP/1.1 404 Not Found\r\n\r\n";
}