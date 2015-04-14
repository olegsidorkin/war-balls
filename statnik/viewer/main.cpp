#include <iostream>
#include <string>
#include "../auxiliary/arguments.h"
#include "../client/viewer.h"

int main (int argc, char *argv[]) {
    Arguments arguments {argc, argv};
    Client::ViewerClient client (arguments);
    arguments.that_is_all();
    client.make_globally_stoppable();
    client.run();

    return 0;
}
