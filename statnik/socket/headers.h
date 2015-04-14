#pragma once

#include "../auxiliary/os.h"

#ifdef OS_UNIX
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

typedef int SOCKET; // C++03
const SOCKET INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
int (*closesocket)(int) = close;
#endif // OS_UNIX

#ifdef OS_WINDOWS
#include <winsock2.h>
#endif // OS_WINDOWS
