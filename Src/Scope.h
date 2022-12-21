#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class ErrorLogger;
class SymbolTable;

struct Symbol //TODO: chnage types to an enum as thatll probably be better than compairing strings all the time
{
	std::string varName;
	int memoryOffset{};			// Its offset (in memory) to the base pointer 
	std::string varType;		// The type of the variable
	size_t varLine{};				// The line of code where the variable is declared
	bool isUsed{false};				// Sort of dirty flag checks whether the variable is used in the code
	bool isCorrect{}; 			// False when a dummy struct is returned to avoid bad cast
	bool noConst{};				// Tell if the variable can be set as a simple const
	int* constPtr{};  			// Const pointer

	/// Returns a string that represents the location of the variable
	/// @return "memOffset(%rbp)" */
	std::string GetOffsetReg() const { return std::to_string(memoryOffset) + "(%rbp)"; }
};

struct Function {
	std::string funcName;
	std::string returnType;
	size_t nbParameters;							// The number of input parameters
	std::vector<std::string> parameterTypes;	// The type of every input parameter
	std::vector<std::string> parameterNames; 	// The names of every parameter
	size_t funcLine; 								// The line of code where the function is declared
	bool isCalled{false};								// Sort of dirty flag checks whether the function is called used for optimazation later
	bool hasFuncCall{ false };						// Whether or not the function contains a function call
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

	Symbol* AddSymbol(const std::string& name, const std::string& varType, size_t varLine, int* constPtr = nullptr);
	Function* AddFunc(const std::string& name, std::string retType, size_t nbParams, std::vector<std::string> paramTypes, std::vector<std::string> paramNames, size_t funcLine);

	const Scope* GetEnclosingScope() const;

	bool HasReturned() const;
	void SetReturned(bool ret);

	int GetStackPointer() const;
	void SetStackPointer(int sP);

	// Static Analysis
	void CheckUnusedSymbols(ErrorLogger& errorLogger);
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

	std::map<std::string_view, int> m_TypeSizes{ {"int", 4}, {"char", 1} };

};

