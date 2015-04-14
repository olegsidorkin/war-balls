#pragma once

#if defined __unix__
#define OS_UNIX
#elif defined _WIN32
#define OS_WINDOWS
#else
ERROR: INCORRECT OS
#endif
