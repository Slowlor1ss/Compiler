#pragma once
#include <stdexcept>
#include <format>
#include <source_location>

//https://stackoverflow.com/questions/42939299/notimplementedexception-c
class NotImplementedException : public std::logic_error
{
public:
    using logic_error::logic_error;

    NotImplementedException(
        const std::source_location location = std::source_location::current())
        : logic_error{ std::format("{} is not implemented!", location.function_name()) }
    {}
};

