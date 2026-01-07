#include <iostream>
#include <thread>
#include "websocket_client.hpp"
#include "url_mgt.hpp"

int main(int argc, char **argv)
{
    std::string url = "wss://echo.websocket.events";
    if (argc == 3 && std::string(argv[1]) == "--url")
        url = argv[2];
    UrlParts u{};
    if (!parseUrl(url, u))
    {
        std::cout << "Invalid URL format\n";
        return 1;
    }
    if (!u.secure)
    {
        std::cout << "This client supports wss:// only\n";
        return 1;
    }
    SimpleWebSocketClient client;
    std::cout << "Connecting to " << url << "...\n";
    if (!client.connect(u.host, u.port, u.path))
    {
        std::cout << "Connection failed. Press Enter.\n";
        std::cin.get();
        return 1;
    }
    std::cout << "Connected. Type messages, 'exit' to quit.\n";
    std::thread receiver([&]()
                         {
        std::string msg;
        while (client.receiveText(msg))
            std::cout << "\n[Server] " << msg << "\n> "; });

    std::string input;
    std::cout << "> ";
    while (std::getline(std::cin, input))
    {
        if (input == "exit")
            break;
        client.sendText(input);
        std::cout << "> ";
    }
    client.close();
    receiver.join();
    return 0;
}
