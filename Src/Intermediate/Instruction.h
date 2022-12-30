#pragma once
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Scope.h"

struct Symbol;
class BasicBlock;
class Scope;
class SymbolTable;

class Instruction
{
	struct InstrPriority //TODO: implement or remove
	{
		int latency;
		int priority;
	};

public:
	enum class OperationType
	{
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
		ConditionalJump,
		UnconditionalJump,
		_LAST = UnconditionalJump
	};

	virtual ~Instruction() = default;

	virtual bool PropagateConst() = 0; // Constant folding -> //https://en.wikipedia.org/wiki/Constant_folding
	virtual void GenerateASM(std::ostream& o) = 0;

	OperationType GetOp() const { return m_OperationType; }
	void SetBasicBlock(BasicBlock* bb) { m_BasicBlock = bb; }
	static constexpr int GetAmountOfRegisters() { return m_AmountOfRegisters; }

protected:
	explicit Instruction(Scope* scope, OperationType op)
		: m_BasicBlock(nullptr), m_Scope(scope), m_OperationType(op)
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
	const OperationType m_OperationType;

	// TODO: quite confusing maybe go over this again later
	// Registers used for calling functions and storing parameters of functions
	// https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
	// we stop at r9 as according to the System V AMD64 ABI (Application Binary Interface),
	// the R9 register and below are non-volatile, meaning they are not modified by function calls and their values are preserved across function calls.
	// The R10 register and above are volatile, meaning they may be modified by function calls and their values are not preserved across function calls.
	static constexpr int m_AmountOfRegisters{ 6 }; //TODO: maybe give more accurate name
	using VartypeToRegisterMap = std::unordered_map<std::string, std::array<std::string, m_AmountOfRegisters>>;
	inline static const VartypeToRegisterMap m_TypeRegisterMap{
		{ "int", { "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d" }},
		{ "char", { "%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b" }}
	};
};

namespace Operation
{

#pragma region FunctionOperations
	//https://en.wikipedia.org/wiki/Function_prologue_and_epilogue
	class Prologue : public Instruction
	{
	public:
		Prologue(Scope* scope)
			: Instruction(scope, OperationType::Prologue)
		{}

		bool PropagateConst() override { return false; }
		void GenerateASM(std::ostream& o) override;

	};

	class Return : public Instruction
	{
	public:
		// Return param can be either a symbol name or a value (or empty when returning nothing)
		Return(std::string returnParam, Scope* scope)
			: Instruction(scope, OperationType::Return), m_ReturnParam(std::move(returnParam))
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		std::string m_ReturnParam;
	};

	class Call : public Instruction
	{
	public:
		Call(const std::string& tempResultName, std::string uniqueFuncName, size_t amountOfParams, Scope* scope)
			: Call(scope->GetSymbol(tempResultName), std::move(uniqueFuncName), amountOfParams, scope)
		{}
		Call(const Symbol* param, std::string uniqueFuncName, size_t amountOfParams, Scope* scope)
			: Instruction(scope, OperationType::Call)
			, m_Param(param)
			, m_UniqueFuncName(std::move(uniqueFuncName))
			, m_AmountOfParams(amountOfParams)
		{}

		bool PropagateConst() override { return false; }
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_Param;
		std::string m_UniqueFuncName;
		size_t m_AmountOfParams;
	};

	class WriteParam : public Instruction
	{
	public:
		WriteParam(const std::string& paramName, const size_t paramIdx, Scope* scope)
			: WriteParam(scope->GetSymbol(paramName), paramIdx ,scope)
		{}
		WriteParam(const Symbol* sym, const size_t paramIdx, Scope* scope)
			: Instruction(scope, OperationType::WriteParam)
			, m_Sym(sym)
			, m_ParamIdx(paramIdx)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_Sym;
		const size_t m_ParamIdx;
		std::optional<int> m_ConstVal{}; // Used in case the return value is const so we can save that value without it changing
	};

	class ReadParam : public Instruction
	{
	public:
		ReadParam(const std::string& paramName, const size_t paramIdx, Scope* scope)
			: ReadParam(scope->GetSymbol(paramName), paramIdx, scope)
		{}
		ReadParam(const Symbol* sym, const size_t paramIdx, Scope* scope)
			: Instruction(scope, OperationType::WriteParam)
			, m_Sym(sym)
			, m_ParamIdx(paramIdx)
		{}

		bool PropagateConst() override { return false; }
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_Sym;
		const size_t m_ParamIdx;
	};
#pragma endregion FunctionOperations

	class WriteConst : public Instruction
	{
	public:
		WriteConst(const std::string& symName, std::string valueSting, Scope* scope)
			: WriteConst(scope->GetSymbol(symName), std::move(valueSting), scope)
		{}
		WriteConst(const Symbol* sym, std::string valueSting, Scope* scope)
			: Instruction(scope, OperationType::WriteConst)
			, m_Sym(sym)
			, m_ValueString(std::move(valueSting))
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_Sym;
		std::string m_ValueString;
	};

	class Assign : public Instruction
	{
	public:
		Assign(const std::string& destSymName, std::string& sourceSymName, Scope* scope)
			: Assign(scope->GetSymbol(destSymName), scope->GetSymbol(sourceSymName), scope)
		{}
		Assign(Symbol* destSym, Symbol* sourceSym, Scope* scope)
			: Instruction(scope, OperationType::Assign)
			, m_DestSym(destSym)
			, m_SourceSym(sourceSym)
		{}

		~Assign() override { std::cout << "Destructor called"; }

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		Symbol* m_DestSym;
		Symbol* m_SourceSym;
	};

	class Declaration : public Instruction
	{
	public:
		//ex. int a, b, c;
		Declaration(const std::string& symName, Scope* scope)
			: Declaration(scope->GetSymbol(symName), scope)
		{}
		Declaration(const Symbol* sym, Scope* scope)
			: Instruction(scope, OperationType::Declaration)
			, m_Sym(sym)
		{}

		bool PropagateConst() override;
		// Purposefully empty
		void GenerateASM(std::ostream& o) override {}

	private:
		const Symbol* m_Sym;
	};

	class Negate : public Instruction
	{
	public:
		Negate(const std::string& tempSymName, const std::string& origSymName, Scope* scope)
			: Negate(scope->GetSymbol(tempSymName), scope->GetSymbol(origSymName), scope)
		{}
		Negate(Symbol* tempSym, Symbol* origSym, Scope* scope)
			: Instruction(scope, OperationType::Negate)
			, m_TempSym(tempSym)
			, m_OrigSym(origSym)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		Symbol* m_TempSym;
		const Symbol* m_OrigSym;
	};

#pragma region Additive
	class Additive : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		Additive(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: Additive(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		Additive(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_ResultSym(resultSym)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		bool CheckObsoleteExpr() const;

		Symbol* m_ResultSym;
		const Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class Plus : public Additive
	{
	public:
		Plus(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Additive(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Plus)
		{}
		Plus(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Additive(resultSym, lhsSym, rhsSym, scope, OperationType::Plus)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class Minus : public Additive
	{
	public:
		Minus(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Additive(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Minus)
		{}
		Minus(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Additive(resultSym, lhsSym, rhsSym, scope, OperationType::Minus)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion Additive

#pragma region Multiplicative
	class Multiplicative : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		Multiplicative(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: Multiplicative(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		Multiplicative(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_ResultSym(resultSym)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		Symbol* m_ResultSym;
		const Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class Mul : public Multiplicative
	{
	public:
		Mul(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Multiplicative(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Mul)
		{}
		Mul(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Multiplicative(resultSym, lhsSym, rhsSym, scope, OperationType::Mul)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class Div : public Multiplicative
	{
	public:
		Div(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Multiplicative(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Div)
		{}
		Div(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Multiplicative(resultSym, lhsSym, rhsSym, scope, OperationType::Div)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class Mod : public Multiplicative
	{
	public:
		Mod(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Multiplicative(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Mod)
		{}
		Mod(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Multiplicative(resultSym, lhsSym, rhsSym, scope, OperationType::Mod)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion Multiplicative

#pragma region AdditiveEqual
	class AdditiveEqual : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		AdditiveEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: AdditiveEqual(scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		AdditiveEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class PlusEqual : public AdditiveEqual
	{
	public:
		PlusEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: AdditiveEqual(lhsSymName, rhsSymName, scope, OperationType::PlusEqual)
		{}
		PlusEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: AdditiveEqual(lhsSym, rhsSym, scope, OperationType::PlusEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class MinusEqual : public AdditiveEqual
	{
	public:
		MinusEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: AdditiveEqual(lhsSymName, rhsSymName, scope, OperationType::MinusEqual)
		{}
		MinusEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: AdditiveEqual(lhsSym, rhsSym, scope, OperationType::MinusEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion AdditiveEqual

#pragma region MultiplicativeEqual
	class MultiplicativeEqual : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		MultiplicativeEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: MultiplicativeEqual(scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		MultiplicativeEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class MulEqual : public MultiplicativeEqual
	{
	public:
		MulEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: MultiplicativeEqual(lhsSymName, rhsSymName, scope, OperationType::MulEqual)
		{}
		MulEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: MultiplicativeEqual(lhsSym, rhsSym, scope, OperationType::MulEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class DivEqual : public MultiplicativeEqual
	{
	public:
		DivEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: MultiplicativeEqual(lhsSymName, rhsSymName, scope, OperationType::DivEqual)
		{}
		DivEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: MultiplicativeEqual(lhsSym, rhsSym, scope, OperationType::DivEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion MultiplicativeEqual

#pragma region Bitwise
	class Bitwise : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		Bitwise(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: Bitwise(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		Bitwise(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_ResultSym(resultSym)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		Symbol* m_ResultSym;
		const Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class BitwiseAnd : public Bitwise
	{
	public:
		BitwiseAnd(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Bitwise(resultSymName, lhsSymName, rhsSymName, scope, OperationType::BitwiseAnd)
		{}
		BitwiseAnd(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Bitwise(resultSym, lhsSym, rhsSym, scope, OperationType::BitwiseAnd)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class BitwiseOr : public Bitwise
	{
	public:
		BitwiseOr(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Bitwise(resultSymName, lhsSymName, rhsSymName, scope, OperationType::BitwiseOr)
		{}
		BitwiseOr(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Bitwise(resultSym, lhsSym, rhsSym, scope, OperationType::BitwiseOr)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class BitwiseXor : public Bitwise
	{
	public:
		BitwiseXor(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Bitwise(resultSymName, lhsSymName, rhsSymName, scope, OperationType::BitwiseXor)
		{}
		BitwiseXor(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Bitwise(resultSym, lhsSym, rhsSym, scope, OperationType::BitwiseXor)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion Bitwise

#pragma region Comparison
	class Comparison : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		Comparison(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, OperationType op)
			: Comparison(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
		{}
		Comparison(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, OperationType op)
			: Instruction(scope, op)
			, m_ResultSym(resultSym)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		Symbol* m_ResultSym;
		const Symbol* m_LhsSym;
		const Symbol* m_RhsSym;
	};

	class LessThan : public Comparison
	{
	public:
		LessThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName,scope, OperationType::LessThan)
		{}
		LessThan(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::LessThan)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class GreaterThan : public Comparison
	{
	public:
		GreaterThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope, OperationType::GreaterThan)
		{}
		GreaterThan(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::GreaterThan)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class Equal : public Comparison
	{
	public:
		Equal(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope, OperationType::Equal)
		{}
		Equal(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::Equal)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class NotEqual : public Comparison
	{
	public:
		NotEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope, OperationType::NotEqual)
		{}
		NotEqual(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::NotEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class LessOrEqual : public Comparison
	{
	public:
		LessOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope, OperationType::LessOrEqual)
		{}
		LessOrEqual(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::LessOrEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class GreaterOrEqual : public Comparison
	{
	public:
		GreaterOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope, OperationType::GreaterOrEqual)
		{}
		GreaterOrEqual(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope, OperationType::GreaterOrEqual)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
	};

	class Not : public Instruction
	{
	public:
		Not(const std::string& tempSymName, const std::string& origSymName, Scope* scope)
			: Not(scope->GetSymbol(tempSymName), scope->GetSymbol(origSymName), scope)
		{}
		Not(const Symbol* tempSym, const Symbol* origSym, Scope* scope)
			: Instruction(scope, OperationType::ExclamationMark)
			, m_TempSym(tempSym)
			, m_OrigSym(origSym)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_TempSym;
		const Symbol* m_OrigSym;
	};
#pragma endregion Comparison

	class ConditionalJump : public Instruction
	{
	public:
		ConditionalJump(const std::string& conditionSymName, std::string exitTrueLabel, std::string exitFalseLabel, Scope* scope)
			: ConditionalJump(scope->GetSymbol(conditionSymName), std::move(exitTrueLabel), std::move(exitFalseLabel), scope)
		{}
		ConditionalJump(Symbol* conditionSym, std::string exitTrueLabel, std::string exitFalseLabel, Scope* scope)
			: Instruction(scope, OperationType::ConditionalJump)
			, m_ConditionSym(conditionSym)
			, m_ExitTrueLabel(std::move(exitTrueLabel))
			, m_ExitFalseLabel(std::move(exitFalseLabel))
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_ConditionSym;
		const std::string m_ExitTrueLabel;
		const std::string m_ExitFalseLabel;
	};

	class UnconditionalJump : public Instruction
	{
	public:
		UnconditionalJump(std::string exitTrueLabel, Scope* scope)
			: Instruction(scope, OperationType::UnconditionalJump)
			, m_ExitTrueLabel(std::move(exitTrueLabel))
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ExitTrueLabel;
	};

}
