#pragma once

#include <string>
#include "../auxiliary/os.h"
#include "../socket/exceptions.h"
#include "../socket/headers.h"
#include "../socket/initialization.h"
#include "../socket/types.h"

namespace Socket {

class Socket {
    SOCKET sockfd;

    explicit Socket(SOCKET _sockfd)
        : sockfd{_sockfd}
    {}
    Socket(const Socket&) = delete;

public:
    Socket() {
        Sockets::enable();

        sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET) {
            throw_from("socket");
        }
    }
    Socket(Socket&& from) {
        sockfd = from.sockfd;
        from.sockfd = INVALID_SOCKET;
    }

    operator bool() const {
        return sockfd != INVALID_SOCKET;
    }
    void bind(Address address, Port port) {
        sockaddr addr = make_sockaddr(address, port);
        if (::bind(sockfd, &addr, sizeof(addr)) == SOCKET_ERROR) {
            throw_from("bind");
        }
    }
    void listen(int number = SOMAXCONN) {
        if (::listen(sockfd, number) == SOCKET_ERROR) {
            throw_from("listen");
        }
    }
    Socket accept() {
        SOCKET new_sockfd = ::accept(sockfd, nullptr, nullptr);
        return std::move(Socket(new_sockfd));
    }
    void connect(Address address, Port port) {
        sockaddr addr = make_sockaddr(address, port);
        if (::connect(sockfd, &addr, sizeof(addr)) == SOCKET_ERROR) {
            throw_from("connect");
        }
    }
    void set_blocking_mode(bool is_blocking) {
#ifdef OS_UNIX
        int flags = fcntl(sockfd, F_GETFL);
        if (::fcntl(sockfd, F_SETFL, is_blocking ? flags &!O_NONBLOCK : flags | O_NONBLOCK) != 0) {
            throw_from("fcnlt");
        };
#endif // OS_UNIX

#ifdef OS_WINDOWS
        u_long mode = !is_blocking; // 1 = non-blocking mode
        if (::ioctlsocket(sockfd, FIONBIO, &mode) != 0) {
            throw_from("ioctlsocket");
        }
#endif // OS_WINDOWS
    }
    int send(const char* message, size_t size) {
        int result = ::send(sockfd, message, size, 0);
        if (result == SOCKET_ERROR || result == 0) { // 0 = something strange
            throw_from("send");
        } else {
            return result;
        }
    }
    int receive(char* buffer, size_t size) {
        int result = ::recv(sockfd, buffer, size, 0);
        if (result == SOCKET_ERROR || result == 0) { // 0 = gracefully closed
            throw_from("recv");
        } else {
            return result;
        }
    }

    ~Socket() {
		if (sockfd != INVALID_SOCKET) {
			::closesocket(sockfd);
		}
    }
};

} // namespace Socket
