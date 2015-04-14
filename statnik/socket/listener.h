#pragma once

#include <stdexcept>
#include "../socket/exceptions.h"
#include "../socket/protocol.h"
#include "../socket/socket.h"

namespace Socket {

class Listener {
    Socket socket;

    Listener (const Listener&) = delete;

public:
    Listener (Address address, Port port)
        : socket{}
    {
        try {
            socket.bind(address, port);
            socket.listen();
        } catch(Exception socket_exception) {
            std::cerr << "Impossible to use given address or port" << std::endl;
            throw std::invalid_argument(socket_exception.what());
        }
        socket.set_blocking_mode(false);
    }
    Listener (Listener&&) = default;

    operator bool() const {
        return socket;
    }
    GameProtocol accept() {
        return GameProtocol{std::move(socket.accept())};
    }
};

} // namespace Socket
