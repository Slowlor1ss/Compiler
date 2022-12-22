#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class BasicBlock;
class Scope;
class SymbolTable;

//TODO: change to base class and have enums be a class
class Instruction
{
	struct InstrPriority
	{
		int latency;
		int priority;
	};

public:
	//enum class Operation {
		
		//Prologue,
		//WriteConst,
		//ReadParam,
		//WriteParam,
		//Call,
		//Return,
		//LessThan,
		//GreaterThan,
		//BitwiseAnd,
		//BitwiseOr,
		//BitwiseXor,
		//Equal,
		//NotEqual,
		//LessOrEqual,
		//GreaterOrEqual,
		//Declaration,
		//Assign,
		//Plus,
		//Minus,
		//Mul,
		//Div,
		//Mod,
		//PlusEqual,
		//MinusEqual,
		//MulEqual,
		//DivEqual,
		//ExclamationMark,
		//Negate,
		//_LAST = Negate
	//};

	// Getters / Setters
	//Operation GetOp() const { return m_Op; }
	//std::vector<std::string> GetParams() { return m_Params; }
	virtual ~Instruction() = default;

	void SetBasicBlock(BasicBlock* bb) { m_BasicBlock = bb; }
	virtual void GenerateASM(std::ostream& o) = 0;
	static constexpr int GetAmountOfRegisters() { return m_AmountOfRegisters; }

protected:
	explicit Instruction(Scope* scope)
		: m_BasicBlock(nullptr), m_Scope(scope)
	{}

	static std::pair<std::string, std::string> GetMoveInstr(const std::string& dest, const std::string& source);
	static std::string GetMoveInstr(const std::string& source);

	static std::string FormatInstruction(std::string instr, std::string param1, std::string param2);
	static std::string FormatInstruction(std::string instr, std::string param);
	static std::string FormatInstruction(std::string instr);
	static void AddCommentToPrevInstruction(std::ostream& o, const std::string& comment);
	static int RoundUpToMultipleOf16(int x);

	BasicBlock* m_BasicBlock;
	Scope* m_Scope;
	//Operation m_Op;
	//std::string m_Dest;
	//std::vector<std::string> m_Params;

	static constexpr int m_AmountOfRegisters{ 6 };
	using VartypeToRegisterMap = std::unordered_map<std::string, std::array<std::string, m_AmountOfRegisters>>;
	inline static const VartypeToRegisterMap m_TypeRegisterMap{
		{ "int", { "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d" }},
		{ "char", { "%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b" }}
	};

//#pragma region Instruction operations
//	void Prologue(std::ostream& o);
//	void Return(std::ostream& o);
//	void Call(std::ostream& o);
//	void WriteParam(std::ostream& o);
//	void ReadParam(std::ostream& o);
//	void WriteCost(std::ostream& o);
//	void Assign(std::ostream& o);
//	void Plus(std::ostream& o);
//#pragma endregion Instruction operations
//
//#pragma region ConstInstr
//	inline static const std::string EAX{ "%eax" };
//	inline static const std::string AL{ "%al" };
//
//	inline static const std::string MOVL{ "movl" };
//	inline static const std::string MOVB{ "movb" };
//#pragma endregion ConstInstr

};

namespace Operation
{

	//https://en.wikipedia.org/wiki/Function_prologue_and_epilogue
	class Prologue : public Instruction
	{
	public:
		Prologue(Scope* scope)
			: Instruction(scope)
		{}

		void GenerateASM(std::ostream& o) override;
	};

	class Return : public Instruction
	{
	public:
		Return(std::string returnParam, Scope* scope)
			: Instruction(scope), m_ReturnParam(std::move(returnParam))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ReturnParam;
	};

	class Call : public Instruction
	{
	public:
		Call(std::string tempResultName, std::string uniqueFuncName, size_t amountOfParams, Scope* scope)
			: Instruction(scope)
			, m_TempVarName(std::move(tempResultName))
			, m_UniqueFuncName(std::move(uniqueFuncName))
			, m_AmountOfParams(amountOfParams)
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		std::string m_TempVarName;
		std::string m_UniqueFuncName;
		size_t m_AmountOfParams;
	};

	class WriteParam : public Instruction
	{
	public:
		WriteParam(std::string paramName, const size_t paramIdx, Scope* scope)
			: Instruction(scope), m_ParamName(std::move(paramName)), m_ParamIdx(paramIdx)
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ParamName;
		const size_t m_ParamIdx;
	};

	class ReadParam : public Instruction
	{
	public:
		ReadParam(std::string paramName, const size_t paramIdx, Scope* scope)
			: Instruction(scope), m_ParamName(std::move(paramName)), m_ParamIdx(paramIdx)
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ParamName;
		const size_t m_ParamIdx;
	};

	class WriteConst : public Instruction
	{
	public:
		WriteConst(std::string symName, std::string valueSting, Scope* scope)
			: Instruction(scope), m_SymName(std::move(symName)), m_ValueString(std::move(valueSting))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		std::string m_SymName;
		std::string m_ValueString;
	};

	class Assign : public Instruction
	{
	public:
		Assign(std::string lhsSym, std::string rhsSym, Scope* scope)
			: Instruction(scope), m_LhsParamName(std::move(lhsSym)), m_RhsParamName(std::move(rhsSym))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Declaration : public Instruction
	{
	public:
		Declaration(std::string symType, std::string symName, Scope* scope)
			: Instruction(scope), m_SymType(std::move(symType)), m_SymName(std::move(symName))
		{}

		// Purposefully empty
		void GenerateASM(std::ostream& o) override {}

	private:
		std::string m_SymType;
		std::string m_SymName;
	};

	class Negate : public Instruction
	{
	public:
		Negate(std::string tempSymName, std::string origSymName, Scope* scope)
			: Instruction(scope), m_TempParamName(std::move(tempSymName)), m_OrigParamName(std::move(origSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_TempParamName;
		const std::string m_OrigParamName;
	};

	class Plus : public Instruction
	{
	public:
		Plus(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Minus : public Instruction
	{
	public:
		Minus(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Mul : public Instruction
	{
	public:
		Mul(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Div : public Instruction
	{
	public:
		Div(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Mod : public Instruction
	{
	public:
		Mod(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class PlusEqual : public Instruction
	{
	public:
		PlusEqual(std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class MinusEqual : public Instruction
	{
	public:
		MinusEqual(std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class MulEqual : public Instruction
	{
	public:
		MulEqual(std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class DivEqual : public Instruction
	{
	public:
		DivEqual(std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class BitwiseAnd : public Instruction
	{
	public:
		BitwiseAnd(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class BitwiseOr : public Instruction
	{
	public:
		BitwiseOr(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class BitwiseXor : public Instruction
	{
	public:
		BitwiseXor(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class LessThan : public Instruction
	{
	public:
		LessThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class GreaterThan : public Instruction
	{
	public:
		GreaterThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Equal : public Instruction
	{
	public:
		Equal(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class NotEqual : public Instruction
	{
	public:
		NotEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class LessOrEqual : public Instruction
	{
	public:
		LessOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class GreaterOrEqual : public Instruction
	{
	public:
		GreaterOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Instruction(scope)
			, m_ResultParamName(std::move(resultSymName))
			, m_LhsParamName(std::move(lhsSymName))
			, m_RhsParamName(std::move(rhsSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ResultParamName;
		const std::string m_LhsParamName;
		const std::string m_RhsParamName;
	};

	class Not : public Instruction
	{
	public:
		Not(std::string tempSymName, std::string origSymName, Scope* scope)
			: Instruction(scope), m_TempParamName(std::move(tempSymName)), m_OrigParamName(std::move(origSymName))
		{}

		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_TempParamName;
		const std::string m_OrigParamName;
	};

}
