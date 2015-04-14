#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/constants.h"
#include "../server/data.h"
#include "../socket/protocol.h"

namespace Server {

enum ClientType {
    GAMER,
    VIEWER,
    UNDEFINED_TYPE
};

class Client {
    GameProtocol protocol;
    Server::Data& data;
    ClientType type;
    std::string id;

    Client(const Client&) = delete;

    void set_type(const std::string& type_name) {
        if (type_name == GAMER_MESSAGE) {
            type = GAMER;
        } else if (type_name == VIEWER_MESSAGE) {
            type = VIEWER;
        } else {
            std::cerr << "Incorrect type of client (" << type_name << ")" << std::endl;
            // throw?
        }
    }

public:
    Client(Server::Data& _data, GameProtocol&& _protocol)
        : protocol(std::move(_protocol))
        , data(_data)
        , type(UNDEFINED_TYPE)
        , id()
    {
        set_type(protocol.receive_message());
        if (type == GAMER) {
            id = data.add_client();
            protocol.send_message(id);
        }
    }
    Client(Client&&) = default;

    void send_field_state() {
        protocol.send_message (data.get_field_state());
    }
    void get_acceleration() {
        if (type == GAMER) {
            std::string acceleration;
            try {
                acceleration = protocol.receive_message();
            } catch (Socket::Exception socket_exception) {
                std::cerr << "Impossible to get acceleration from client" << std::endl;
                std::cerr << socket_exception.what() << std::endl;
            }
            data.set_acceleration(id, acceleration);
        }
    }
};

} // namespace Server
