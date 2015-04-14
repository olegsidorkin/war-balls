#pragma once

#include "../auxiliary/os.h"
#include "../socket/exceptions.h"
#include "../socket/headers.h"

namespace Socket {

#ifdef OS_UNIX
class Sockets {
public:
    // not necessary to do anything
    static void enable() {}
};
#endif // OS_UNIX

#ifdef OS_WINDOWS
class Sockets {
    Sockets() {
        WSADATA wsaData {}; // warnings about initializers
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw_from("WSAStartup");
        }
    }
    ~Sockets() {
        WSACleanup();
    }

public:
    static void enable() {
        static Sockets sockets;
    }
};

#endif // OS_WINDOWS

} // namespace Socket
