#include "url_mgt.hpp"

bool parseUrl(const std::string &url, UrlParts &out)
{
    if (url.rfind("wss://", 0) == 0)
    {
        out.secure = true;
        out.port = 443;
        out.host = url.substr(6);
    }
    else if (url.rfind("ws://", 0) == 0)
    {
        out.secure = false;
        out.port = 80;
        out.host = url.substr(5);
    }
    else
    {
        return false;
    }

    size_t slash = out.host.find('/');
    if (slash == std::string::npos)
    {
        out.path = "/";
    }
    else
    {
        out.path = out.host.substr(slash);
        out.host = out.host.substr(0, slash);
    }
    return true;
}
