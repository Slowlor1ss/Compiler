#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>
#include "Intermediate/BasicBlock.h"

class ErrorLogger;
class SymbolTable;

//TODO: maybe make a private constructor and destructor or one that can only be accesed by scope
struct Symbol //TODO: chnage types to an enum as thatll probably be better than compairing strings all the time
{
	std::string varName;			// Defined name for symbol (also used to check if a symbol isn't defined twice)
	std::string varType;			// The type (int, char, void, ...)
	size_t varLine{};				// The lineNr where the symbol was declared
	int memoryOffset{};				// Offset to base pointer (used to find where the value is stored) 
	std::optional<int> constVal{};  // hold the const value if any (this holds the last const value that has been used, (when wanting to know the const value in a basic block use BasicBlock.GetConst() //TODO: make this more clear))
	bool isUsed{false};//TODO: maybe change to isCritical				// Sort of dirty flag checks whether the variable is used in the code

	/// Returns a string that represents the location of the variable
	/// @return "memOffset(%rbp)" */
	std::string GetOffsetReg() const
	{
		if (memoryOffset == 0) //Ignore this, its the result of some unfortunate design chooses and lack of time
			return varName;
		return std::to_string(memoryOffset) + "(%rbp)";
	}
};

struct Function
{
	std::string funcName;						// A unique name for function will be in format of function_type1type2... (allows overloading)
	std::string returnType;
	Scope* scope;								// The scope of the function
	size_t nbParameters;
	std::vector<std::string> parameterTypes;	// List of types of the parameters (in order)
	std::vector<std::string> parameterNames; 	// List of names of every parameter (in order)
	size_t funcLine; 							// The lineNr where the function was declared
	bool isCalled{false};						// Sort of dirty flag checks whether the function is called used for optimization later
	bool isConst{false};		//TODO: remove					// If no operations in this function need to load something from memory it is const (this will be set in propagate const)
	bool hasFuncCall{ false };					// Whether or not the function contains a function call
};

class Scope
{
public:
	explicit Scope(int sP = 0, Scope* parent = nullptr);
	~Scope();

	// Tell whether a variable ... is present in the table
	bool HasSymbol(const std::string& name) const;
	int HasParam(const std::string& name) const;
	bool HasFunc(const std::string& name) const;

	// Get the ... corresponding according to the name if it was found, else return nullptr
	Symbol* GetSymbol(const std::string& name);
	Function* GetFunc(const std::string& name);

	Symbol* AddSymbol(const std::string& name, const std::string& varType, size_t varLine, std::optional<int> constVal = {});
	Symbol* AddTempSymbol(const std::string& name, const std::string& varType, size_t varLine, std::optional<int> constVal = {});
	Function* AddFunc(const std::string& name, std::string retType, size_t nbParams, std::vector<std::string> paramTypes, std::vector<std::string> paramNames, size_t funcLine);

	const Scope* GetEnclosingScope() const;

	bool HasReturned() const;
	void SetReturned(bool ret);

	int GetStackPointer() const;
	void SetStackPointer(int sP);

	// Static Analysis
	void CheckUnusedSymbolsAndResetIsUsed(ErrorLogger& errorLogger);
	bool CheckFunctionRedefinition(const std::string& funcName, size_t lineNumber, ErrorLogger& errorLogger) const;
	bool CheckSymbolRedefinition(const std::string& varName, size_t lineNumber, ErrorLogger& errorLogger) const;
	int GetScopeSize();

private:
	Symbol* AddSymbol(const Symbol& sym);
	Function* AddFunc(const Function& fn);

	int m_StackPointer;
	// Whether the scope has a return statement
	bool m_Returned = false;

	Scope* m_EnclosingScope;
	std::vector<Scope*> m_ChildScopes;

	//Possible optimization could be using a std::string_view as key
	//but as that's me just doing premature optimization and could cause undefined behaviour in a specific event ill leave it for now >:D
	//Found a guy thinking the same as me: https://www.reddit.com/r/cpp_questions/comments/4hvtnr/stdunordered_map_and_stdstring_view/
	std::unordered_map<std::string, Symbol> m_SymbolMap;
	std::unordered_map<std::string, Function> m_FuncMap;

	std::unordered_map<std::string_view, int> m_TypeSizes{ {"int", 4}, {"char", 1} };

};

