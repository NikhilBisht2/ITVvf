#pragma once
#include <string>
#include <vector>

class WebSocketFrame
{
public:
    static std::vector<unsigned char> encode(const std::string &message);
    static bool decode(const std::vector<unsigned char> &frame, std::string &out);
};
