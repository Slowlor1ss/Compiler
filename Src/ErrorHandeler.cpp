#include "ErrorHandeler.h"
#include <cassert>
#include <utility>

void ErrorLogger::Signal(errorType severity, std::string_view  message, size_t lineNumber)
{
	switch (severity) {
	case WARNING:
		m_Warning = true;
		break;
	case ERROR:
		m_Error = true;
		break;
	default: 
		assert(!"should never get here!"); //!"string" evaluates to false
		return;
	}
	GenerateMessage(severity, message, lineNumber);
}

void ErrorLogger::GenerateMessage(int severity, std::string_view message, size_t lineNumber)
{
	std::cerr << m_ErrorValues[severity];
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
