#pragma once

#include <string>
#include <iostream>
#include "../auxiliary/conversions.h"

namespace Server {

struct FieldSize {
    int size;
};
std::istream& operator>> (std::istream& is, FieldSize& field_size) {
    return is >> field_size.size;
}
std::ostream& operator<< (std::ostream& os, const FieldSize& field_size) {
    return os << field_size.size;
}

class Data {
    std::stringstream data;
    std::string field;
    int next_id;

public:
    explicit Data(Arguments& arguments)
		: next_id(0) // C++03
	{
	    auto field_size = arguments.get("field-size", FieldSize{DEFAULT_FIELD_SIZE});
        std::cout << "ServerData(" << field_size << ")" << std::endl;
        // to do
    }
    std::string add_client() {
        std::string id = to_string(next_id++);
        std::cout << "ServerData::add_client() = " << id << std::endl;
        return id;
        // to do
    }
    void set_acceleration(const std::string& id, const std::string& acceleration) {
        std::cout << "ServerData::set_acceleration(id = " << id
            << ", " << acceleration << ")" << std::endl;
        // to do
        data << id << ": " << acceleration << std::endl;
    }
    void update_field_state() {
        field = data.str();
        std::cout << "ServerData::update()" << std::endl;
        // to do
    }
    const std::string& get_field_state() { // returns reference?
        std::cout << "ServerData::field_state() = " << field.size() << " bytes" << std::endl;
        return field;
        // to do
    }
};

} // namespace Server
