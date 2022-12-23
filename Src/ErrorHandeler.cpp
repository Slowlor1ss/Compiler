#include "ErrorHandeler.h"
#include <cassert>
#include <iostream>
#include <utility>
#include "ConsoleColors.h"

void ErrorLogger::Signal(errorType severity, std::string_view message, size_t lineNumber)
{
	switch (severity) {
	case WARNING:
		m_Warning = true;
		std::cerr << KYEL; //set color to yellow
		break;
	case ERROR:
		m_Error = true;
		std::cerr << KRED; //set color to red
		break;
	default: 
		assert(!"should never get here!"); //!"string" evaluates to false
		return;
	}
	GenerateMessage(severity, message, lineNumber);
}

void ErrorLogger::GenerateMessage(int severity, std::string_view message, size_t lineNumber)
{
	std::cerr << '[' << m_ErrorValues[severity] << ']' << RST; //reset color
	std::cerr << " at line " << lineNumber << " : " << message << ".\n";
}

bool ErrorLogger::HasError() const
{
	return m_Error;
}

bool ErrorLogger::HasWarning() const
{
	return m_Warning;
}
