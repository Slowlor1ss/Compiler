#pragma once
#include <string>
#include <vector>
#include "../ErrorHandeler.h"
#include "../Scope.h"

class BasicBlock;
class Scope;
class SymbolTable;
enum class Operation;

class Instruction
{
public:
	enum class Operation {
		//https://en.wikipedia.org/wiki/Function_prologue_and_epilogue
		Prologue,
		WriteConst,
		ReadParam,
		WriteParam,
		Call,
		Return,
		LessThan,
		GreaterThan,
		BitwiseAnd,
		BitwiseOr,
		BitwiseXor,
		Equal,
		NotEqual,
		LessOrEqual,
		GreaterOrEqual,
		Declaration,
		Assign,
		Plus,
		Minus,
		Mul,
		Div,
		Mod,
		PlusEqual,
		MinusEqual,
		MulEqual,
		DivEqual,
		ExclamationMark,
		Negate,
		_LAST = Negate
	};

	Instruction(BasicBlock* bb, Operation op, std::string dest, std::vector<std::string> params, Scope* scope);
	~Instruction() = default;

	// Getters / Setters
	Operation GetOp() const { return m_Op; }
	std::vector<std::string> GetParams() { return m_Params; }
	void GenerateASM(std::ostream& o);

private:

	BasicBlock* m_BasicBlock;
	Scope* m_Scope;
	Operation m_Op;
	std::string m_Dest;
	std::vector<std::string> m_Params;

#pragma region Instruction operations
	void DefaultNotImplementedOperation() { throw NotImplementedException(); }
	void Return(std::ostream& o);
#pragma endregion Instruction operations
};

