#pragma once

#include <iostream>
#include <signal.h>

class Stoppable;

Stoppable* globally_stoppable;

void signal_handler(int);

class Stoppable {
protected:
    bool stop_signal;

public:
    Stoppable()
        : stop_signal(false)
    {}
    void set_stop_signal() {
        stop_signal = true;
    }
    void make_globally_stoppable() {
        globally_stoppable = this;
        signal(SIGINT, signal_handler);
    }
};

void signal_handler(int) {
    if (globally_stoppable) {
        globally_stoppable->set_stop_signal();
    }
    std::cout << "Interruption signal received" << std::endl; // safe?
}
