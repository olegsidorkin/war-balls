#pragma once

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include "../auxiliary/constants.h"

struct Argument {
    std::string name;
    std::string value;
    bool used;

    Argument(const std::string& _name, const std::string& _value = "")
        : name{_name}
        , value{_value}
        , used{false}
    {}
    void add_to_value(const std::string& string) {
        if (value.empty()) {
            value = string;
        } else {
            value += ' ' + string;
        }
    }
};

class Arguments {
    const static char ARGUMENT_SYMBOL;

    std::list<Argument> arguments;
    std::list<std::string> argument_names;

    void show_help_message() {
        std::cout << "Correct argument names are";
        for (std::string& argument_name : argument_names) {
            std::cout << ' ' << argument_name;
        }
        std::cout << std::endl;
    }

public:
    Arguments(int argc, char *argv[])
        : arguments()
        , argument_names()
    {
        for (int i = 1; i < argc; ++i) { // 0 = file name
            if (argv[i][0] == ARGUMENT_SYMBOL) {
                arguments.push_back(Argument {argv[i] + 1});
            } else {
                if (arguments.empty()) {
                    throw std::invalid_argument{"Argument list is starting incorrectly"};
                } else {
                    arguments.back().add_to_value(argv[i]);
                }
            }
        }
    }
    Arguments(std::list<Argument> _arguments)
        : arguments(_arguments)
        , argument_names()
    {}
    template <class Type>
    Type get(const std::string& name, const Type& default_value) {
        argument_names.push_back(name);
        for (Argument& argument : arguments) {
            if (argument.name == name) {
                argument.used = true;
                Type result = from_string (argument.value, default_value);
                std::cout << "Argument " << name << " is " << result << std::endl;
                return result;
            }
        }
        std::cerr << "Argument " << name << " is not found, default is " << default_value << std::endl;
        return default_value;
    }
    void that_is_all() {
        bool need_help_message = false;
        for (Argument& argument : arguments) {
            if (!argument.used) {
                if (argument.name == "help" || argument.name == "h" || argument.name == "?") {
                    need_help_message = true;
                } else {
                    std::cerr << "Argument " << argument.name << " does not exists" << std::endl;
                    need_help_message = true;
                }
            }
        }
        if (need_help_message) {
            show_help_message();
        }
    }
};

const char Arguments::ARGUMENT_SYMBOL = '-'; // C++03
