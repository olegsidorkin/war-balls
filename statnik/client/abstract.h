#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/constants.h"
#include "../auxiliary/stoppable.h"
#include "../socket/protocol.h"

namespace Client {

class AbstractClient : public Stoppable {
protected:
    GameProtocol protocol;

public:
    AbstractClient(Arguments& arguments)
        : Stoppable()
        , protocol(
            arguments.get(ARGUMENT_ADDRESS, Socket::Address{DEFAULT_CLIENT_ADDRESS}),
            arguments.get(ARGUMENT_PORT, Socket::Port{DEFAULT_PORT})
        )
    {}
};

} // namespace Client
