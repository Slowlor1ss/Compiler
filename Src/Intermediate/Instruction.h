#pragma once
#include <array>
#include <string>
#include <vector>
#include "../ErrorHandeler.h"
#include "../Scope.h"

class BasicBlock;
class Scope;
class SymbolTable;
enum class Operation;

//TODO: change to base class and have enums be a class
class Instruction
{
	struct InstrPriority
	{
		int latency;
		int priority;
	};

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

	static constexpr int GetAmountOfRegisters() { return m_AmountOfRegisters; }

private:
	static std::pair<std::string, std::string> GetMoveInstr(const std::string& dest, const std::string& source);
	static std::string GetMoveInstr(const std::string& source);

	static std::string FormatInstruction(std::string instr, std::string param1, std::string param2);
	static std::string FormatInstruction(std::string instr, std::string param);
	static std::string FormatInstruction(std::string instr);
	static void AddCommentToPrevInstruction(std::ostream& o, const std::string& comment);
	static int RoundUpToMultipleOf16(int x);

	BasicBlock* m_BasicBlock;
	Scope* m_Scope;
	Operation m_Op;
	std::string m_Dest;
	std::vector<std::string> m_Params;

	static constexpr int m_AmountOfRegisters{ 6 };
	using VartypeToRegisterMap = std::unordered_map<std::string, std::array<std::string, m_AmountOfRegisters>>;
	inline static const VartypeToRegisterMap m_TypeRegisterMap{
		{ "int", { "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d" }},
		{ "char", { "%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b" }}
	};

#pragma region Instruction operations
	void Prologue(std::ostream& o);
	void Return(std::ostream& o);
	void Call(std::ostream& o);
	void WriteParam(std::ostream& o);
	void ReadParam(std::ostream& o);
	void WriteCost(std::ostream& o);
	void Assign(std::ostream& o);
	void Plus(std::ostream& o);
#pragma endregion Instruction operations

#pragma region ConstInstr
	inline static const std::string EAX{ "%eax" };
	inline static const std::string AL{ "%al" };

	inline static const std::string MOVL{ "movl" };
	inline static const std::string MOVB{ "movb" };
#pragma endregion ConstInstr

};

