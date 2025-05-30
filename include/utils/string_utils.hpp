#pragma once

#include <vector>
#include <string>

std::vector<std::string> splitString(const std::string &str, const std::string &delim);
std::string gzipCompress(const std::string &data);