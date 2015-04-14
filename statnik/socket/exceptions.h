#pragma once

#include <string>
#include "../auxiliary/conversions.h"
#include "../auxiliary/os.h"
#include "../socket/headers.h"

namespace Socket {

class Exception : public std::exception {
    int error_code;
    std::string error_message;

	std::string make_error_description(int error_code) {
#ifdef OS_UNIX
        return strerror(error_code); //no need to deallocate
#endif // OS_UNIX

#ifdef OS_WINDOWS
        char* plain_error_description = nullptr;

        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error_code,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            (LPTSTR)&plain_error_description, // strange, but it works this way
            0,
            nullptr
        );

        if (plain_error_description) {
            std::string error_description (plain_error_description);
            LocalFree(plain_error_description);
            return error_description.substr(0, error_description.size() - 2); // without \r\n
        } else {
            return "unkwnown error";
        }
#endif // OS_WINDOWS
	}

public:
    Exception(const std::string& error_source, int _error_code)
        : error_code (_error_code)
        , error_message (error_source + " failed with " + to_string(error_code) + "\n" + make_error_description(error_code))
    {}
	~Exception() noexcept {} // C++03
    const char* what() const noexcept /*override*/ {
        return error_message.c_str();
    }
};

void throw_from(const std::string& error_place) {
#ifdef OS_UNIX
    int error_code = errno;
#endif // OS_UNIX

#ifdef OS_WINDOWS
    int error_code = WSAGetLastError(); // before throwing, otherwise resets to 2
#endif // OS_WINDOWS

    throw Socket::Exception(error_place, error_code);
}

} // namespace Socket
