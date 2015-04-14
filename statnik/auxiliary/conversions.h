#pragma once

#include <iomanip>
#include <sstream>
#include <string>

template <class T>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <class T>
std::string to_filled_string(const T& value, int width, char fill) {
    std::ostringstream oss;
    oss << std::setw (width) << std::setfill(fill) << value;
    return oss.str();
}

template <class T>
T from_string(const std::string& string, T value = T()) {
    std::istringstream iss (string);
    iss >> value;
    return value;
}
