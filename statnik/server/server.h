#pragma once

#include <iostream>
#include <list>
#include <string>
#include "../auxiliary/conversions.h"
#include "../auxiliary/sleep.h"
#include "../auxiliary/stoppable.h"
#include "../server/client.h"
#include "../socket/listener.h"
#include "../socket/types.h"

namespace Server {

class Server : public Stoppable {
    Socket::Listener listener;
    Data data;
    std::list<Client> clients;
    Milliseconds time_quant;

public:
    Server(Arguments& arguments)
        : Stoppable()
        , listener(
            arguments.get(ARGUMENT_ADDRESS, Socket::Address{DEFAULT_SERVER_ADDRESS}),
            arguments.get(ARGUMENT_PORT, Socket::Port{DEFAULT_PORT})
        )
        , data(arguments)
        , clients()
        , time_quant(
            arguments.get(ARGUMENT_TIME_QUANT, Milliseconds{DEFAULT_TIME_QUANT})
        )
    {}

    void sleep() {
        if (!stop_signal) {
            std::cout << "Sleeping..." << std::endl;
            sleep_for(time_quant);
        }
    }
    void add_clients() {
        std::cout << "Adding clients (now " << clients.size() << ")..." << std::endl;
        while (true) {
            GameProtocol new_protocol = listener.accept();
            if (new_protocol) {
                Client client (data, std::move(new_protocol));
                clients.push_back(std::move(client));
            } else {
                break;
            }
        }
    }
    void update_field_state() {
        std::cout << "Updating field..." << std::endl;
        data.update_field_state();
    }
    void send_field_state() {
        std::cout << "Sending to clients..." << std::endl;
        for (auto it = clients.begin(); it != clients.end(); ) {
            try {
                it->send_field_state ();
                ++it;
            } catch (Socket::Exception socket_exception) {
                std::cerr << "Impossible to send message to client" << std::endl;
                std::cerr << socket_exception.what() << std::endl;
                std::cerr << "Deleting client..." << std::endl;
                clients.erase (it++);
            }
        }
    }
    void get_accelerations() {
        std::cout << "Getting accelerations..." << std::endl;
        for (auto& client : clients) { // wait?
            client.get_acceleration();
        }
    }
    void run() {
        while (!stop_signal) {
            add_clients();
            update_field_state();
            send_field_state();
            sleep();
            get_accelerations();
            sleep();
        }
    }
};

} // namespace Server
