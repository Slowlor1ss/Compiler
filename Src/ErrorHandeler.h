#pragma once
#include <stdexcept>
#include <format>
#include <iostream>
#include <source_location>
#include "Intermediate/Instruction.h"

//https://stackoverflow.com/questions/42939299/notimplementedexception-c
class NotImplementedException final : public std::logic_error
{
public:
    using logic_error::logic_error;

    NotImplementedException(const std::source_location location = std::source_location::current())
        : logic_error{ std::format("{} is not implemented!", location.function_name()) }
    {}
};

class Cricket_Error : public std::runtime_error
{
public:
    Cricket_Error(const std::string& what, size_t lineNr)
        : runtime_error{ what }
		, m_Line(lineNr)
    {}

    _NODISCARD virtual size_t lineNr() const
    {
        return m_Line;
    }

private:
    size_t m_Line;
};

class UnImplementedInstr_Exception final : public std::runtime_error
{
public:

    UnImplementedInstr_Exception(const std::string& operationName, const std::source_location location = std::source_location::current())
        : runtime_error{ std::format("{} in {} is not implemented!", operationName, location.function_name()) }
    {}
};

class Redefenition_Error : public Cricket_Error
{
public:
    explicit Redefenition_Error(std::string_view name, size_t lineNr)
        : Cricket_Error{ std::format("{} has already been defined or declared", name), lineNr }
    {}
};

class FunctionRedefenition final : public Redefenition_Error
{
public:
	explicit FunctionRedefenition(std::string_view funcName, size_t lineNr)
        : Redefenition_Error{ std::format("Function '{}'", funcName), lineNr }
    {}
};

class SymbolRedefenition final : public Redefenition_Error
{
public:
	explicit SymbolRedefenition(std::string_view funcName, size_t lineNr)
        : Redefenition_Error{ std::format("Symbol '{}'", funcName), lineNr }
    {}
};

class Unsupported_Expression : public Cricket_Error
{
public:
    explicit Unsupported_Expression(std::string_view message, size_t lineNr)
        : Cricket_Error{ std::format("Unsupported expression '{}'", message), lineNr }
    {}
};

class Unknown_Expression : public Cricket_Error
{
public:
    explicit Unknown_Expression(std::string_view what, size_t lineNr)
        : Cricket_Error{ std::format("Unknown {}", what), lineNr }
    {}
};

class Unknown_FunctionCall : public Unknown_Expression
{
public:
    explicit Unknown_FunctionCall(std::string_view name, size_t lineNr)
        : Unknown_Expression{ std::format("Function call '{}'", name), lineNr }
    {}
};

class Unknown_Symbol : public Unknown_Expression
{
public:
    explicit Unknown_Symbol(std::string_view name, size_t lineNr)
        : Unknown_Expression{ std::format("Symbol '{}'", name), lineNr }
    {}
};

class ArgumentsAmountMismatch : public Cricket_Error
{
public:
    explicit ArgumentsAmountMismatch(std::string_view funcName, size_t lineNr)
        : Cricket_Error{ std::format("Wrong number of arguments in function call '{}'", funcName), lineNr }
    {}
};


enum errorType { ERROR, WARNING };

/**
 * \brief Used for logging errors and others + sending these to the cerr stream
 */
class ErrorLogger final
{
public:
    void Signal(errorType severity, std::string_view  message, size_t lineNumber);
    bool HasError() const;
    bool HasWarning() const;

private:
	static const constexpr char* const m_ErrorValues[2]{ "ERROR", "WARNING" };

    bool m_Error = false;
    bool m_Warning = false;

    void GenerateMessage(int severity, std::string_view  message, size_t lineNumber);

};
