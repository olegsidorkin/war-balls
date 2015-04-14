#pragma once

#include <string>
#include "../auxiliary/arguments.h"
#include "../socket/protocol.h"

const int DEFAULT_PORT = 54321;
const int DEFAULT_SERVER_ADDRESS = INADDR_ANY;
const int DEFAULT_CLIENT_ADDRESS = INADDR_LOOPBACK;
const int DEFAULT_TIME_QUANT = 500;
const int DEFAULT_FIELD_SIZE = 50;
const std::string DEFAULT_STRATEGY_NAME = "random";
const std::string ARGUMENT_ADDRESS = "address";
const std::string ARGUMENT_PORT = "port";
const std::string ARGUMENT_TIME_QUANT = "time-quant";
const std::string ARGUMENT_FIELD_SIZE = "field-size";
const std::string GAMER_MESSAGE = "gamer";
const std::string VIEWER_MESSAGE = "viewer";
typedef Socket::Protocol<5, ' '> GameProtocol; // C++03
