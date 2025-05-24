#include "utils/string_utils.hpp"
#include <zlib.h>

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

std::string gzipCompress(const std::string &data)
{
    z_stream zs{};
    deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);

    zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(data.data()));
    zs.avail_in = data.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    do
    {
        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);
        outstring.append(outbuffer, sizeof(outbuffer) - zs.avail_out);
    } while (ret == Z_OK);

    deflateEnd(&zs);

    return outstring;
}