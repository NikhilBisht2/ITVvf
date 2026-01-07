#include "websocket_client.hpp"
#include "utils.hpp"
#include "websocket_frame.hpp"
#include <iostream>
#include <vector>
#include <ws2tcpip.h>
#include <openssl/err.h>
#include <openssl/rand.h>

bool SimpleWebSocketClient::connect(
    const std::string &host,
    int port,
    const std::string &path)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0)
        return false;
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET)
        return false;
    if (::connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
        return false;
    freeaddrinfo(res);
    SSL_library_init();
    SSL_load_error_strings();
    ssl_ctx = SSL_CTX_new(TLS_client_method());
    ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, (int)sock);
    SSL_set_tlsext_host_name(ssl, host.c_str());
    if (SSL_connect(ssl) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    unsigned char key_raw[16];
    RAND_bytes(key_raw, sizeof(key_raw));
    std::string ws_key = utils::base64(key_raw, sizeof(key_raw));
    std::string handshake =
        "GET " + path + " HTTP/1.1\r\n"
                        "Host: " +
        host + "\r\n"
               "Upgrade: websocket\r\n"
               "Connection: Upgrade\r\n"
               "Sec-WebSocket-Version: 13\r\n"
               "Sec-WebSocket-Key: " +
        ws_key + "\r\n\r\n";
    if (!utils::write_all(ssl, handshake.c_str(), (int)handshake.size()))
        return false;
    std::string response;
    char buf[1024];
    while (response.find("\r\n\r\n") == std::string::npos)
    {
        int n = SSL_read(ssl, buf, sizeof(buf));
        if (n <= 0)
            return false;
        response.append(buf, n);
    }
    if (response.find("101 Switching Protocols") == std::string::npos)
    {
        std::cerr << "Handshake rejected:\n"
                  << response << "\n";
        return false;
    }
    return true;
}

bool SimpleWebSocketClient::sendText(const std::string &message)
{
    auto frame = WebSocketFrame::encode(message);
    return utils::write_all(ssl, frame.data(), (int)frame.size());
}

bool SimpleWebSocketClient::receiveText(std::string &out)
{
    unsigned char hdr[2];
    if (SSL_read(ssl, hdr, 2) <= 0)
        return false;
    size_t len = hdr[1] & 0x7F;
    std::vector<char> payload(len);
    size_t read = 0;
    while (read < len)
    {
        int r = SSL_read(ssl, payload.data() + read, (int)(len - read));
        if (r <= 0)
            return false;
        read += r;
    }
    out.assign(payload.begin(), payload.end());
    return true;
}

void SimpleWebSocketClient::close()
{
    if (ssl)
    {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    if (ssl_ctx)
        SSL_CTX_free(ssl_ctx);

    if (sock != INVALID_SOCKET)
        closesocket(sock);

    WSACleanup();
}
