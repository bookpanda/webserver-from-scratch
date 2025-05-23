#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utils/string_utils.hpp"

int main(int argc, char **argv)
{
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Logs from your program will appear here!\n";

  // socket(IPv4, TCP, default TCP protocol)
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
  {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
  {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;         // IPv4
  server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
  server_addr.sin_port = htons(4221);       // Port 4221

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5; // max no. of pending connections
  if (listen(server_fd, connection_backlog) != 0)
  {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  while (true)
  {
    // accept() blocks until a client connects
    int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
    if (client_socket < 0)
    {
      std::cerr << "Failed to accept connection\n";
      continue;
    }

    // get client request
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0)
    {
      std::cerr << "Failed to receive data\n";
      close(client_socket);
      continue;
    }
    buffer[bytes_received] = '\0'; // Null-terminate the received data

    std::vector<std::string> requestParts = splitString(buffer, "\r\n");
    const std::string requestLine = requestParts[0];
    std::vector<std::string> lineParts = splitString(requestLine, " ");
    const std::string method = lineParts[0];
    const std::string path = lineParts[1];

    std::cout << "Received request:\n"
              << buffer << "\n";
    std::cout << "Method: " << method << "\n";
    std::cout << "Path: " << path << "\n";

    std::string response = "HTTP/1.1 200 OK\r\n\r\n";
    if (path != "/")
    {
      response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    send(client_socket, response.c_str(), response.size(), 0); // 0 = no flags (default)
    close(client_socket);
  }

  close(server_fd);

  return 0;
}
