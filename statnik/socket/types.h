#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/conversions.h"
#include "../socket/headers.h"

namespace Socket {

class Address {
    u_long host_order_address;

public:
    explicit Address (u_long _host_order_address)
        : host_order_address (_host_order_address)
    {}
    u_long get_host_order_address() const {
        return host_order_address;
    }
    u_long get_network_order_address() const {
        return htonl(host_order_address);
    }
    void set_host_order_address(u_long _host_order_address) {
        host_order_address = _host_order_address;
    }
    void set_network_order_address(u_long _network_order_address) {
        host_order_address = ntohl(_network_order_address);
    }
};
std::istream& operator>> (std::istream& is, Address& address) {
    std::string string;
    std::getline(is, string);
    address.set_network_order_address(inet_addr(string.c_str()));
    return is;
}
std::ostream& operator<< (std::ostream& os, const Address& address) {
    in_addr addr;
    addr.s_addr = address.get_network_order_address();
    return os << inet_ntoa(addr);
}

class Port {
    u_short host_order_port;

public:
    explicit Port (int _host_order_port)
        : host_order_port (_host_order_port)
    {}
    explicit Port (const std::string& string)
        : host_order_port(from_string<u_short>(string))
    {}
    u_short get_host_order_port() const {
        return host_order_port;
    }
    u_short get_network_order_port() const {
        return htons(host_order_port);
    }
    friend std::istream& operator>> (std::istream& is, Port& port) {
        is >> port.host_order_port;
        return is;
    }
};
std::ostream& operator<< (std::ostream& os, const Port& port) {
    return os << port.get_host_order_port();
}

sockaddr make_sockaddr(Address address, Port port) {
    sockaddr addr;
    sockaddr_in* addr_in = static_cast<sockaddr_in*> (static_cast<void*> (&addr));
    addr_in->sin_family = AF_INET;
    addr_in->sin_port = port.get_network_order_port();
    addr_in->sin_addr.s_addr = address.get_network_order_address();
    return addr;
}

} // namespace Socket
