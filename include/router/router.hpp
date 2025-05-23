#pragma once
#include <string>
#include <map>

std::string handleRequest(const std::string &method, const std::string &path, const std::map<std::string, std::string> &headers, const std::string &body);