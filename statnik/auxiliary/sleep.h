#pragma once

#include <iostream>
#include <string>
#include "../auxiliary/os.h"

#ifdef OS_UNIX
#include <unistd.h>
#endif // OS_UNIX

#ifdef OS_WINDOWS
#include <windows.h>
#endif // OS_WINDOWS

struct Milliseconds {
    int ms;

    explicit Milliseconds (int _ms = 0)
        : ms {_ms}
    {}
    int get_milliseconds() const {
        return ms;
    }
    int get_microseconds() const {
        return ms * 1000;
    }
};
std::istream& operator>> (std::istream& is, Milliseconds& ms) {
    return is >> ms.ms;
}
std::ostream& operator<< (std::ostream& os, const Milliseconds& ms) {
    return os << ms.get_milliseconds() << " ms";
}

void sleep_for(Milliseconds time) {
#ifdef OS_UNIX
    usleep(time.get_microseconds());
#endif // OS_UNIX

#ifdef OS_WINDOWS
    Sleep(time.get_milliseconds());
#endif // OS_WINDOWS
}
