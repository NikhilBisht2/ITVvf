#pragma once
#include <string>
#include <winsock2.h>
#include <openssl/ssl.h>

class SimpleWebSocketClient
{
public:
    bool connect(const std::string &host, int port, const std::string &path);
    bool sendText(const std::string &message);
    bool receiveText(std::string &out);
    void close();

private:
    SOCKET sock = INVALID_SOCKET;
    SSL_CTX *ssl_ctx = nullptr;
    SSL *ssl = nullptr;
};
