#include "router/router.hpp"
#include "utils/string_utils.hpp"
#include <format>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "http/response_builder.hpp"

std::string handleRequest(const std::string &method, const std::string &path, const std::map<std::string, std::string> &headers, const std::string &body)
{
    HttpResponseBuilder builder;
    auto it = headers.find("Accept-Encoding");
    if (it != headers.end())
    {
        builder.setContentEncoding(it->second);
    }

    if (method == "GET" && path == "/")
    {
        std::string response = builder.build();
        return response;
    }

    const std::vector<std::string> pathParts = splitString(path, "/");
    if (pathParts.size() > 1 && pathParts[1] == "echo")
    {
        std::string response = builder.setBody(pathParts[2]).build();
        return response;
    }
    else if (pathParts.size() > 1 && pathParts[1] == "user-agent")
    {
        std::string response = builder.setBody(headers.at("User-Agent")).build();
        return response;
    }
    else if (pathParts.size() > 1 && pathParts[1] == "files")
    {
        if (method == "GET")
        {

            std::string filePath = pathParts[2];
            std::ifstream file(filePath, std::ios::binary); // open file in binary mode
            if (!file)
            {
                std::string response = builder.setStatus(http::Status::NotFound).build();
                return response;
            }
            std::ostringstream oss;
            oss << file.rdbuf(); // read file content into string stream
            std::string fileContent = oss.str();

            std::string response = builder.setBody(fileContent).build();
            return response;
        }
        else if (method == "POST")
        {
            std::string newFilePath = pathParts[2];
            std::ofstream file(newFilePath, std::ios::binary); // open file in binary mode
            if (!file.is_open())
            {
                std::string response = builder.setStatus(http::Status::InternalServerError).build();
                return response;
            }

            file.write(body.c_str(), body.size());
            file.close();

            std::string response = builder.setStatus(http::Status::Created).build();
            return response;
        }
    }

    std::string response = builder.setStatus(http::Status::NotFound).build();
    return response;
}