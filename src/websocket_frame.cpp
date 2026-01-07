#include "websocket_frame.hpp"
#include <openssl/rand.h>

std::vector<unsigned char> WebSocketFrame::encode(const std::string &message)
{
    std::vector<unsigned char> frame;
    frame.push_back(0x81);
    size_t len = message.size();
    if (len <= 125)
    {
        frame.push_back(0x80 | (unsigned char)len);
    }
    else
    {

        frame.push_back(0x80 | (unsigned char)len);
    }
    unsigned char mask[4];
    RAND_bytes(mask, 4);
    frame.insert(frame.end(), mask, mask + 4);
    for (size_t i = 0; i < len; ++i)
        frame.push_back(message[i] ^ mask[i % 4]);
    return frame;
}

bool WebSocketFrame::decode(const std::vector<unsigned char> &frame, std::string &out)
{
    if (frame.size() < 6)
        return false;
    size_t len = frame[1] & 0x7F;
    const unsigned char *mask = &frame[2];
    for (size_t i = 0; i < len; ++i)
    {
        out.push_back(frame[6 + i] ^ mask[i % 4]);
    }
    return true;
}
