#include "utils/string_utils.hpp"

std::vector<std::string> splitString(const std::string &str, const std::string &delim)
{
    std::vector<std::string> tokens;
    size_t start = 0, end;
    while ((end = str.find(delim, start)) != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.length();
    }

    if (start < str.length())
        tokens.push_back(str.substr(start));

    return tokens;
}