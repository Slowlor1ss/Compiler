#pragma once
#include <stack>
#include <string>
#include <vector>
class Scope;

//used this for as reference for making my symbol table system https://stackoverflow.com/questions/15588452/build-symbol-table-from-grammar/15602385#15602385

class SymbolTable
{
public:
    SymbolTable();

    //Scope* PushScope();
    Scope* PushScope(Scope* parentScope);
    void PopScope();

    Scope* CurrentScope() const { return m_SymbolTableStack.top(); }
    Scope* GetGlobalScope() const { return m_GlobalScope; }

private:
    Scope* m_GlobalScope;
    //This includes both the global symbol table and the scoped symbol tables
    std::stack<Scope*> m_SymbolTableStack;
};

