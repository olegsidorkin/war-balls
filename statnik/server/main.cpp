#include <iostream>
#include <string>
#include "../auxiliary/arguments.h"
#include "../server/server.h"

int main (int argc, char *argv[]) {
    Arguments arguments (argc, argv);
    Server::Server server (arguments);
    arguments.that_is_all();
    server.make_globally_stoppable();
    server.run();

    return 0;
}
