#pragma once

#include <string>
#include "../auxiliary/conversions.h"
#include "../socket/socket.h"

namespace Socket {

template <int HEADER_SIZE, char HEADER_FILL>
class Protocol {
    Socket socket;

    Protocol (const Protocol&) = delete;

    void send_string(const std::string& string) {
        const char* buffer = string.c_str();
        size_t current_size = 0;
        while (current_size < string.size()) {
            current_size += socket.send(buffer + current_size, string.size() - current_size);
        }
    }
    std::string receive_string(const size_t size) {
        char buffer[size]; // warning about variable length
        size_t current_size = 0;
        while (current_size < size) {
            current_size += socket.receive(buffer + current_size, size - current_size);
        }
        return std::string(buffer, current_size);
    }

public:
    explicit Protocol (Socket&& _socket)
        : socket(std::move(_socket))
    {}
    Protocol (Address address, Port port)
        : socket()
    {
        socket.connect(address, port);
    }
    Protocol (Protocol&&) = default;

    operator bool() const {
        return socket;
    }
    void send_message(const std::string& message) {
        std::string header = to_filled_string(message.size(), HEADER_SIZE, HEADER_FILL);
        send_string(header + message);
    }
    std::string receive_message() {
        size_t size = from_string<size_t>(receive_string(HEADER_SIZE));
        return receive_string(size);
    }
};

} // namespace Socket
