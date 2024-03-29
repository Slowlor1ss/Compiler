#include "Scope.h"

#include <utility>
#include "ErrorHandeler.h"
#include "SymbolTable.h"

Scope::Scope(const int sP, Scope* parent) : m_StackPointer(sP), m_EnclosingScope(parent)
{
    if (m_EnclosingScope != nullptr) 
    {
        m_EnclosingScope->m_ChildScopes.push_back(this);
    }
}

Scope::~Scope()
{
}

bool Scope::HasSymbol(const std::string& name) const
{
    return (m_SymbolMap.contains(name) 
        || (m_EnclosingScope != nullptr && m_EnclosingScope->HasSymbol(name)));
}

int Scope::HasParam(const std::string& name) const
{
    throw NotImplementedException();
}

bool Scope::HasFunc(const std::string& name) const
{
    return (m_FuncMap.contains(name)
        || (m_EnclosingScope != nullptr && m_EnclosingScope->HasFunc(name)));
}

Symbol* Scope::GetSymbol(const std::string& name)
{
    //TODO: check in the parameter list
    //if (HasParam(name)) {
    //    return param;
    //}

    if (const auto it = m_SymbolMap.find(name); it != m_SymbolMap.end()) {
        return &it->second;
    }
    else if (m_EnclosingScope != nullptr)
    {
        return m_EnclosingScope->GetSymbol(name);
    }

	return nullptr; //TODO!: maybe throw error here and remove all then useless checks but watch ut where the check and where the get happens
}


Function* Scope::GetFunc(const std::string& name)
{
    if (const auto it = m_FuncMap.find(name); it != m_FuncMap.end())
    {
        return &it->second;
    }
    else if(m_EnclosingScope != nullptr)
    {
	    return m_EnclosingScope->GetFunc(name);
    }

    return nullptr;
}

Symbol* Scope::AddSymbol(const Symbol& sym)
{
    m_StackPointer -= m_TypeSizes[sym.varType];
    return &(m_SymbolMap[sym.varName] = sym);
}

Symbol* Scope::AddSymbol(const std::string& name, const std::string& varType, size_t varLine, std::optional<int> constVal)
{
    m_StackPointer -= m_TypeSizes[varType];
    const Symbol sym{ name, varType, varLine, m_StackPointer, constVal, false };
    return &(m_SymbolMap[name] = sym);
}

Symbol* Scope::AddTempSymbol(const std::string& name, const std::string& varType, size_t varLine, std::optional<int> constVal)
{
    const Symbol sym{ name, varType, varLine, 0, constVal, false };
    return &(m_SymbolMap[name] = sym);
}

Function* Scope::AddFunc(const Function& fn)
{
    return &(m_FuncMap[fn.funcName] = fn);
}

Function* Scope::AddFunc(const std::string& name, std::string retType, size_t nbParams, std::vector<std::string> paramTypes, std::vector<std::string> paramNames, size_t funcLine)
{
	const Function fn{
            name, std::move(retType), nullptr, nbParams, std::move(paramTypes), std::move(paramNames), funcLine
	};

    return &(m_FuncMap[name] = fn);
}

const Scope* Scope::GetEnclosingScope() const
{
    return m_EnclosingScope;
}

bool Scope::HasReturned() const
{
    return m_Returned;
}

void Scope::SetReturned(bool ret)
{
    m_Returned = ret;
}

int Scope::GetStackPointer() const
{
    return m_StackPointer;
}

void Scope::SetStackPointer(int sP)
{
    m_StackPointer = sP;
}

// Checks if this symbol has been used in the obvious sense (meaning: if its defined and then never used) if not then send a warning
// Then we also reset this is used boolean to be later used in deadcode elimination where only the critical symbols get maked as used
void Scope::CheckUnusedSymbolsAndResetIsUsed(ErrorLogger& errorLogger)
{
    for (auto& [symbolName, symbol] : m_SymbolMap)
    {
        //if (!symbol.isUsed) 
        //{
	       // std::string message = "Unused parameter \"" + symbolName + "\" at line " + std::to_string(symbol.varLine);
        //    errorLogger.Signal(WARNING, message, symbol.varLine);
        //}

        symbol.isUsed = false;
    }
}

// Return whether or not function already exists
bool Scope::CheckFunctionRedefinition(const std::string& funcName, size_t lineNumber, ErrorLogger& errorLogger) const
{
    if (this->HasFunc(funcName))
    {
        const std::string message = std::format("Function \"{}\" has already been defined or declared", funcName);
        errorLogger.Signal(ERROR, message, lineNumber);
        return true;
    }
    return false;
}

bool Scope::CheckSymbolRedefinition(const std::string& varName, size_t lineNumber, ErrorLogger& errorLogger) const
{
    if (this->HasSymbol(varName))
    {
        const std::string message = std::format("Symbol \"{}\" has already been defined or declared", varName);
        errorLogger.Signal(ERROR, message, lineNumber);
        return true;
    }
    return false;
}

int Scope::GetScopeSize()
{
    int size = 0;

    // Calculate size of symbol table
    for (const auto& s : m_SymbolMap)
    {
	    if (s.second.memoryOffset != 0)
			size += m_TypeSizes[s.second.varType];
    }

    // Add size of child tables
    for (Scope* sT : m_ChildScopes)
    {
        size += sT->GetScopeSize();
    }

    return size;
}
