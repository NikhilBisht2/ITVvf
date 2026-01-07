#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "websocket_frame.hpp"

static void assert_true(bool cond, const char *msg)
{
    if (!cond)
        throw std::runtime_error(msg);
}

void test_frame_encoding_basic()
{
    auto frame = WebSocketFrame::encode("Hello");
    assert_true(frame[0] == 0x81, "Opcode incorrect");
    assert_true((frame[1] & 0x7F) == 5, "Payload length incorrect");
    assert_true(frame[1] & 0x80, "Mask bit not set");
    assert_true(frame.size() == 2 + 4 + 5, "Frame size incorrect (header + mask + payload)");
}

void test_frame_roundtrip()
{
    std::string original = "Hello World";
    auto frame = WebSocketFrame::encode(original);
    std::string decoded;
    bool ok = WebSocketFrame::decode(frame, decoded);
    assert_true(ok, "Failed to decode frame");
    assert_true(decoded == original, "Decoded payload mismatch");
}

void run_framing_tests()
{
    test_frame_encoding_basic();
    test_frame_roundtrip();
}

int main()
{
    try
    {
        run_framing_tests();
        std::cout << "All tests passed!" << std::endl;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}

