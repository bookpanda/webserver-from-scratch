#include "router/router.hpp"
#include "utils/string_utils.hpp"
#include <format>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

const std::string HTTP_OK = "HTTP/1.1 200 OK\r\n";
const std::string HTTP_CREATED = "HTTP/1.1 201 Created\r\n";
const std::string HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n";
const std::string HTTP_INTERNAL_SERVER_ERROR = "HTTP/1.1 500 Internal Server Error\r\n";

const std::string CONTENT_TEXT = "Content-Type: text/plain\r\n";
const std::string CONTENT_OCTET = "Content-Type: application/octet-stream\r\n";
const std::string CONTENT_LENGTH = "Content-Length: ";

std::string handleRequest(const std::string &method, const std::string &path, const std::map<std::string, std::string> &headers, const std::string &body)
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
    else if (pathParts.size() > 1 && pathParts[1] == "files")
    {
        if (method == "GET")
        {

            std::string filePath = pathParts[2];
            std::ifstream file(filePath, std::ios::binary); // open file in binary mode
            if (!file)
            {
                return HTTP_NOT_FOUND + "\r\n";
            }
            std::ostringstream oss;
            oss << file.rdbuf(); // read file content into string stream
            std::string fileContent = oss.str();

            std::string contentLength = CONTENT_LENGTH + std::to_string(fileContent.size());
            return std::format("{}{}{}\r\n\r\n{}", HTTP_OK, CONTENT_OCTET, contentLength, fileContent);
        }
        else if (method == "POST")
        {
            std::string newFilePath = pathParts[2];
            std::ofstream file(newFilePath, std::ios::binary); // open file in binary mode
            if (!file.is_open())
            {
                return HTTP_INTERNAL_SERVER_ERROR + "\r\n";
            }

            file.write(body.c_str(), body.size());
            file.close();

            return HTTP_CREATED + "\r\n";
        }
    }

    return HTTP_NOT_FOUND + "\r\n";
}