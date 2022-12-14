#include "SymbolTable.h"
#include "Scope.h"

//Scope* SymbolTable::PushScope()
//{
//	const auto s = new Scope(0, m_GlobalScope);
//    m_SymbolTableStack.emplace(s);
//    return s;
//}

SymbolTable::SymbolTable() : m_GlobalScope(new Scope())
{
    m_SymbolTableStack.push(m_GlobalScope);
}

Scope* SymbolTable::PushScope(Scope* parentScope)
{
    const auto s = new Scope(parentScope->GetStackPointer(), parentScope);
    //just gonna paste this once for all emplace (back's) https://stackoverflow.com/questions/10890653/why-would-i-ever-use-push-back-instead-of-emplace-back
    m_SymbolTableStack.emplace(s);
    return s;
}

void SymbolTable::PopScope()
{
    //maybe watch out about poping the global scope as this also deletes and thus m_GlobalScope would be a dangeling pointer
#ifdef _DEBUG
    auto* oldTop = m_SymbolTableStack.top();
#endif

    m_SymbolTableStack.pop();

#ifdef _DEBUG
    oldTop = nullptr;
#endif
}
