#pragma once
#include <string>
#include <openssl/ssl.h>

namespace utils
{
    std::string base64(const unsigned char *data, int len);
    bool write_all(SSL *ssl, const void *data, int len);
}
