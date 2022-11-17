#pragma once
#include <string>
#include <vector>

class Scope;
class SymbolTable;
enum Operation;

class Instruction
{
public:
	Instruction(Scope* scope, Operation op, std::vector<std::string> params, SymbolTable* sT);

	// Getters / Setters
	Operation GetOp() const { return m_Op; }
	std::vector<std::string> GetParams() { return m_Params; }

private:

	Scope* m_Scope;
	Operation m_Op;
	// Params of the instruction (usually src, dest, tmpVar)
	std::vector<std::string> m_Params;

public:
	enum Operation {
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
	};
};

