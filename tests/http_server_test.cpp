#include <catch2/catch_test_macros.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

std::string send_http_request(const std::string &request, const std::string &host = "127.0.0.1", int port = 4221)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    REQUIRE(sock >= 0);

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    int res = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    REQUIRE(res == 0);

    ssize_t sent = send(sock, request.c_str(), request.size(), 0);
    REQUIRE(sent == (ssize_t)request.size());

    char buffer[4096];
    std::string response;

    ssize_t received;
    while ((received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[received] = '\0';
        response += buffer;
        // Simple heuristic: if response contains "\r\n\r\n" and length > 0, break
        if (response.find("\r\n\r\n") != std::string::npos && received < (ssize_t)sizeof(buffer) - 1)
            break;
    }

    close(sock);

    return response;
}

TEST_CASE("GET / returns 200 OK", "[http]")
{
    std::string request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
}

TEST_CASE("GET /foo returns 404 Not Found", "[http]")
{
    std::string request =
        "GET /foo HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 404 Not Found") != std::string::npos);
}

TEST_CASE("GET /echo returns 200 OK", "[http]")
{
    std::string request =
        "GET /echo/abc HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    REQUIRE(response.find("abc") != std::string::npos);
}

TEST_CASE("GET /user-agent returns 200 OK", "[http]")
{
    std::string request =
        "GET /user-agent HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "User-Agent: TestClient/1.0\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    REQUIRE(response.find("TestClient/1.0") != std::string::npos);
    REQUIRE(response.find("Content-Length: 14") != std::string::npos);
}

TEST_CASE("GET /echo Accept-Encoding: gzip returns 200 OK with Content-Encoding: gzip", "[http]")
{
    std::string request =
        "GET /echo/abc HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Accept-Encoding: gzip\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    REQUIRE(response.find("Content-Encoding: gzip") != std::string::npos);
}

TEST_CASE("GET /echo Accept-Encoding: invalid-encoding returns 200 OK w/o Content-Encoding", "[http]")
{
    std::string request =
        "GET /echo/abc HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Accept-Encoding: invalid-encoding\r\n"
        "Connection: close\r\n"
        "\r\n";

    std::string response = send_http_request(request);
    REQUIRE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    REQUIRE(response.find("Content-Encoding: ") == std::string::npos);
}
