#pragma once

#include <string>
#include <format>
#include "constants.hpp"
#include "utils/string_utils.hpp"
class HttpResponseBuilder
{
private:
    http::Status status = http::Status::OK;
    http::ContentType contentType = http::ContentType::Text;
    std::string contentEncoding = "";
    std::string body = "";

public:
    HttpResponseBuilder &setStatus(const http::Status &newStatus)
    {
        status = newStatus;
        return *this;
    }

    HttpResponseBuilder &setContentType(const http::ContentType &newContentType)
    {
        contentType = newContentType;
        return *this;
    }

    HttpResponseBuilder &setBody(const std::string &bodyContent)
    {
        body = bodyContent;
        return *this;
    }

    HttpResponseBuilder &setContentEncoding(const std::string &encoding)
    {
        contentEncoding = encoding;
        return *this;
    }

    std::string build() const
    {
        std::string lengthHeader = "Content-Length: " + std::to_string(body.size()) + "\r\n";
        std::string response = http::to_string(status) + http::to_string(contentType) + lengthHeader;

        std::vector<std::string> encodings = splitString(contentEncoding, ", ");
        for (const auto &encoding : encodings)
        {
            if (encoding == "gzip")
            {
                response += "Content-Encoding: gzip\r\n";
                break;
            }
        }

        return response + "\r\n" + body;
    }
};
