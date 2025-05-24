#pragma once

#include <string>

namespace http
{
    enum class Status
    {
        OK,
        Created,
        NotFound,
        InternalServerError
    };

    inline std::string to_string(Status s)
    {
        switch (s)
        {
        case Status::OK:
            return "HTTP/1.1 200 OK\r\n";
        case Status::Created:
            return "HTTP/1.1 201 Created\r\n";
        case Status::NotFound:
            return "HTTP/1.1 404 Not Found\r\n";
        case Status::InternalServerError:
            return "HTTP/1.1 500 Internal Server Error\r\n";
        default:
            return "HTTP/1.1 500 Internal Server Error\r\n"; // fallback
        }
    }

    enum class ContentType
    {
        Text,
        Octet
    };

    inline std::string to_string(ContentType t)
    {
        switch (t)
        {
        case ContentType::Text:
            return "Content-Type: text/plain\r\n";
        case ContentType::Octet:
            return "Content-Type: application/octet-stream\r\n";
        default:
            return "Content-Type: text/plain\r\n";
        }
    }
}
