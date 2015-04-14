#pragma once

#include <string>
#include <iostream>
#include "../auxiliary/conversions.h"

namespace Gamer {

struct StrategyName {
    std::string name;
};
std::istream& operator>> (std::istream& is, StrategyName& strategy_name) {
    return is >> strategy_name.name;
}
std::ostream& operator<< (std::ostream& os, const StrategyName& strategy_name) {
    return os << strategy_name.name;
}

class Data {
    std::string id;

public:
    Data(Arguments& arguments)
		: id()
	{
	    auto strategy_name = arguments.get("strategy-name", StrategyName{DEFAULT_STRATEGY_NAME});
        std::cout << "GamerData(" << strategy_name << ")" << std::endl;
        // to do
    }
    void set_id(const std::string& _id) {
        std::cout << "GamerData::set_id(" << _id << ")" << std::endl;
        id = _id;
        // to do
    }
    std::string get_id() {
        return id; // temporary for emulator
    }
    std::string get_acceleration(const std::string& field) {
		std::string acceleration = to_string(field.size()) + ' ' + to_string(field.size()); // temporary
        std::cout << "GamerData::get_acceleration(" << field.size() << " bytes) = " << acceleration << std::endl;
        return acceleration;
        // to do
    }
};

} // namespace Gamer
