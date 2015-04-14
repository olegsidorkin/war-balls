#include <string>
#include <iostream>
#include <vector>
#include "../auxiliary/arguments.h"
#include "../server/data.h"
#include "../gamer/data.h"

class Emulator {
    Server::Data server_data;
    std::list<Gamer::Data> gamers_data;

public:
    Emulator(Arguments& arguments)
        : server_data(arguments)
        , gamers_data()
    {}
    void add_client(Arguments& arguments) {
        gamers_data.push_back(Gamer::Data(arguments));
        std::string id = server_data.add_client();
        gamers_data.back().set_id(id);
    }
    void next_turn() {
        server_data.update_field_state();
        for (auto& gamer_data : gamers_data) {
            server_data.set_acceleration (
                gamer_data.get_id(),
                gamer_data.get_acceleration(
                    server_data.get_field_state()
                )
            );
        }
    }
};

int main() {
    Argument field_size {"field_size", "50"};
    Arguments arguments {std::list<Argument> {field_size}};
    Emulator emulator {arguments};

    for (int i = 0; i < 5; ++i) {
        Argument strategy_name {"strategy-name", "random"};
        Arguments arguments {std::list<Argument> {strategy_name}};
        emulator.add_client(arguments);
    }

    while (true) {
        std::cin.get();
        emulator.next_turn();
    }
}
