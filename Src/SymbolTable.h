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

    void Init();

    Scope* PushScope();

    void PopScope();

	Scope* CurrentScope();
    Scope* GetScope(int genId);

    int NextGenId() {
        m_GenId++;
        return m_GenId;
    }

    std::string ToString();

private:
    //TODO: maybe make stack
    std::vector<Scope*> m_Scopes;
    int m_GenId;
};

