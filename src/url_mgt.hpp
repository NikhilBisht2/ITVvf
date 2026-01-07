#pragma once
#include <string>

struct UrlParts
{
    bool secure;
    std::string host;
    int port;
    std::string path;
};

bool parseUrl(const std::string &url, UrlParts &out);
