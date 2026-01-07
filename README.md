Overview

ITVvf is a minimal Secure WebSocket (wss://) client written in C++, implemented close to the protocol level.
It demonstrates how to establish a TLS connection, perform a WebSocket handshake, and exchange masked WebSocket text frames without using high-level WebSocket libraries.

The client connects to a WebSocket server, sends user-typed messages, and prints responses received from the server in real time.

Key Characteristics

Client-side WebSocket implementation (RFC 6455 – partial)

TLS (wss://) support using OpenSSL

Manual WebSocket handshake

Manual WebSocket frame encoding and decoding

Interactive terminal-based client

Lightweight and dependency-minimal (no Boost, no external WS libs)

Supported Features

Secure WebSocket connections (wss:// only)

Text frame transmission

Client-side masking

Concurrent receive loop using std::thread

URL parsing for WebSocket endpoints
