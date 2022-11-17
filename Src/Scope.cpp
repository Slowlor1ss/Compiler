#include "Scope.h"
#include "ErrorHandeler.h"
#include "SymbolTable.h"

Scope::Scope(const int sP, Scope* par) : m_StackPointer(sP), m_EnclosingScope(par)
{
    if (m_EnclosingScope != nullptr) 
    {
        m_EnclosingScope->m_ChildScopes.push_back(this);
    }
}

Scope::~Scope()
{
    for (const auto& var : m_SymbolMap) 
    {
        delete(var.second.constPtr);
    }
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
        m_EnclosingScope->GetSymbol(name);
    }

	return nullptr;
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

void Scope::AddSymbol(const Symbol& sym)
{
    m_StackPointer -= m_TypeSizes[sym.varType];
    m_SymbolMap[sym.varName] = sym;
}

void Scope::AddFunc(const Function& fn)
{
    m_FuncMap[fn.funcName] = fn;
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
