#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/constants.h"
#include "../client/abstract.h"
#include "../gamer/data.h"
#include "../socket/protocol.h"

namespace Client {

class GamerClient : public AbstractClient {
    Gamer::Data data;

public:
    GamerClient(Arguments& arguments)
        : AbstractClient(arguments)
        , data(arguments)
    {
        protocol.send_message(GAMER_MESSAGE);
        std::string id = protocol.receive_message();
        std::cout << "Connected with id " << id << std::endl;
        data.set_id(id);
    }

    void run() {
        while (!stop_signal) {
            std::string field = protocol.receive_message();
            std::string acceleration = data.get_acceleration(field);
            protocol.send_message(acceleration);
        }
    }
};

} // namespace Gamer
