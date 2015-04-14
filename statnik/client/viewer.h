#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/constants.h"
#include "../client/abstract.h"
#include "../socket/protocol.h"

namespace Client {

class ViewerClient : public AbstractClient {

public:
    ViewerClient(Arguments& arguments)
        : AbstractClient(arguments)
    {
        protocol.send_message(VIEWER_MESSAGE);
    }
    void run() {
        while (!stop_signal) {
            std::string field = protocol.receive_message();
            std::cout << "// ========== //" << std::endl << field << std::endl;
            // to do
        }
    }
};

} // namespace Client
