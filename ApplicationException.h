#pragma once

#include <stdexcept>

namespace FW
{
    class ApplicationException : public std::runtime_error
    {
    public:
        ApplicationException(const std::string& message) : std::runtime_error(message) {}
        ApplicationException(const char* message) : std::runtime_error(message) {}
    };
}