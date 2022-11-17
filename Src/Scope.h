#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Instruction.h"
class SymbolTable;

struct Symbol
{
	std::string_view varName;
	int memoryOffset{};			// Its offset (in memory) to the base pointer 
	std::string_view varType;		// The type of the variable
	int varLine{};				// The line of code where the variable is declared
	bool isUsed{};				// Sort of dirty flag checks whether the variable is used in the code
	bool isCorrect{}; 			// False when a dummy struct is returned to avoid bad cast
	bool noConst{};				// Tell if the variable can be set as a simple const
	int* constPtr{};  			// Const pointer
};

struct Function {
	std::string_view funcName;
	std::string_view returnType;
	int nbParameters;							// The number of input parameters
	std::vector<std::string_view> parameterTypes;	// The type of every input parameter
	std::vector<std::string_view> parameterNames; 	// The names of every parameter
	int funcLine; 								// The line of code where the function is declared
	bool isCalled;								// Sort of dirty flag checks whether the function is called used for optimazation later
};

class Scope
{
public:
	explicit Scope(int sP = 0, Scope* par = nullptr);
	~Scope();

	// Tell whether a variable ... is present in the table
	bool HasSymbol(const std::string& name) const;
	int HasParam(const std::string& name) const;
	bool HasFunc(const std::string& name) const;

	// Get the ... corresponding according to the name if it was found, else return nullptr
	Symbol* GetSymbol(const std::string& name);
	Function* GetFunc(const std::string& name);

	void AddSymbol(const Symbol& sym);
	void AddFunc(const Function& fn);

	const Scope* GetEnclosingScope() const;

	bool HasReturned() const;
	void SetReturned(bool ret);

	int GetStackPointer() const;
	void SetStackPointer(int sP);

private:

	int m_StackPointer;
	// Whether the scope has a return statement
	bool m_Returned = false;

	Scope* m_EnclosingScope;
	std::vector<Scope*> m_ChildScopes;

	std::unordered_map<std::string_view, Symbol> m_SymbolMap;
	std::unordered_map<std::string_view, Function> m_FuncMap;

	std::unordered_map<std::string_view, int> m_TypeSizes{ {"int", 4}, {"char", 1} };

};

