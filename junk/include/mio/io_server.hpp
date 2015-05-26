#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <chrono>
#include <atomic>

#include "mio.hpp"
#include "connection.hpp"
#include "server_socket.hpp"

namespace mio {

struct ServerConfig {
    std::string address;
    int port;

    ServerConfig(int port = 8992, const std::string& address = "37.143.13.206") :
        address(address),
        port(port)
        {}
};

class EpollDescriptorManager {
private:
    static constexpr size_t MAX_EVENTS = 1024;
    Socket epoll_socket_;

    struct epoll_event events_[MAX_EVENTS];
    size_t events_ready_count_;

public:
    class EpollEvent {
    private:
        struct epoll_event *event_;
        int epoll_socket_;

    public:
        EpollEvent(struct epoll_event *event, int epoll_socket) :
            event_(event),
            epoll_socket_(epoll_socket)
            {}

        bool error() {
            return (event_->events & EPOLLERR ||
                    event_->events & EPOLLHUP);
        }

        std::string getErrorMessage(int fd) {
            int       error = 0;
            socklen_t errlen = sizeof(error);

            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *)&error, &errlen) == 0) {
                return std::string(std::strerror(error));
            } else {
                return std::string("Error");
            }
        }

        bool closed() {
            return (event_->events & EPOLLRDHUP);
        }

        bool output() {
            return (event_->events & EPOLLOUT);
        }

        bool input() {
            return (event_->events & EPOLLIN);
        }

        template<typename T>
        T getData() {
            assert(sizeof(T) <= sizeof(event_->data.u64));
            return *(T *) (&event_->data.u64);
        }
    };

    class EpollEventIterator {
    private:
        struct epoll_event *event_seek_;
        int epoll_socket_;

    public:
        EpollEventIterator(struct epoll_event *event_seek, int epoll_socket) :
            event_seek_(event_seek),
            epoll_socket_(epoll_socket)
            {}

        EpollEvent operator *() const {
            return EpollEvent(event_seek_, epoll_socket_);
        }

        EpollEventIterator &operator ++() {
            event_seek_++;
            return *this;
        }

        bool operator != (const EpollEventIterator &other) {
            return event_seek_ != other.event_seek_;
        }
    };

    void getReadyDescriptors() {
        events_ready_count_ = epoll_wait(epoll_socket_.getDescriptor(),
                events_,
                MAX_EVENTS,
                -1);
    }

    EpollEventIterator begin() {
        return EpollEventIterator(events_, epoll_socket_.getDescriptor());
    }

    EpollEventIterator end() {
        return EpollEventIterator(events_ + events_ready_count_, epoll_socket_.getDescriptor());
    }

    EpollDescriptorManager() :
        epoll_socket_(epoll_create1(0)) {
        memset(events_, 0, sizeof(events_));
    }

    template<typename T>
    void addWatchedDescriptor(int fd, T data) {
        struct epoll_event event;
        memset(&event, 0, sizeof(event));

        assert(sizeof(data) <= sizeof(event.data.u64));

        *(T *) &event.data.u64 = data;

        event.events = EPOLLIN;
        event.events |= EPOLLOUT;
        event.events |= EPOLLRDHUP;

        auto result = epoll_ctl(epoll_socket_.getDescriptor(),
                EPOLL_CTL_ADD,
                fd,
                &event);

        if (result == -1) {
            throw std::runtime_error("Fail to add watched socket");
        }
    }
};

template<typename DescriptorManager>
class IOServer {
private:
    DescriptorManager socket_manager_;

    std::list<std::shared_ptr<Connection>> connections_;
    using ConnectionIter = std::list<std::shared_ptr<Connection>>::iterator;
    std::atomic<bool> stop_;

public:
    std::shared_ptr<Connection> addConnection(std::shared_ptr<Connection> connection) {
        auto iter = connections_.insert(connections_.begin(), connection);
        assert(sizeof(iter) <= sizeof(uint64_t));
        socket_manager_.addWatchedDescriptor(connection->getDescriptor(), iter);
        return connection;
    }

    void closeConnection(ConnectionIter connection) {
        std::shared_ptr<Connection> inner = *connection;
        (*connection)->onClose();
        connections_.erase(connection);
    }

    IOServer() :
        connections_(),
        stop_(false)
        {}

    void eventLoop() {
        while (!stop_) {
            socket_manager_.getReadyDescriptors();
            for (auto event: socket_manager_) {
                auto connection = event.template getData<ConnectionIter>();

                if (event.error()) {
                    std::cerr << event.getErrorMessage((*connection)->getDescriptor()) << std::endl;
                    closeConnection(connection);
                    continue;

                } else if (event.input()) {
                    std::cout << "input" << std::endl;
                    (*connection)->onInput();
                } else if (event.output()) {
                    std::cout << "output" << std::endl;
                    (*connection)->onOutput();
                } else if (event.closed()) {
                    std::cout << "closed" << std::endl;
                    closeConnection(connection);
                    continue;
                }
                if ((*connection)->needClose()) {
                    std::cout << "needClose" << std::endl;
                    closeConnection(connection);
                    continue;
                }
            }
        }
    }

    void stop() {
        stop_ = true;
    }
};

} // namespace mio
