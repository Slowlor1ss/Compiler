#pragma once
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../Scope.h"
#include <variant>

struct Symbol;
class BasicBlock;
class SymbolTable;

class Instruction
{
	struct InstrPriority //TODO: implement or remove
	{
		int latency;
		int priority;
	};

public:
	//TODO: probably remove enum
	/*enum class InstructionTag
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
	};*/

	enum class InstructionTag
	{
		Prologue,
		WriteConst,
		ReadParam,
		WriteParam,
		Call,
		Return,

		Declaration,
		Assign,

		ConditionalJump,
		UnconditionalJump,

		UnaryOperation,
		BinaryOperation,
		CompoundAssignment,

	};

	virtual ~Instruction() = default;

	virtual bool PropagateConst() = 0; // Constant folding -> //https://en.wikipedia.org/wiki/Constant_folding
	virtual void DeadCodeElimination(bool allowSetUnused) = 0;
	virtual void GenerateASM(std::ostream& o) = 0;

	//InstructionTag GetOp() const { return m_InstructionTag; }
	InstructionTag GetTag() const { return m_Tag; }
	bool GetIsUsed() const { return m_IsUsed; }
	void SetBasicBlock(BasicBlock* bb) { m_BasicBlock = bb; }
	static constexpr int GetAmountOfRegisters() { return m_AmountOfRegisters; }

protected:
	explicit Instruction(Scope* scope, InstructionTag op)
		: m_BasicBlock(nullptr), m_Scope(scope), m_Tag(op)
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
	const InstructionTag m_Tag;
	bool m_IsUsed = false;

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
			: Instruction(scope, InstructionTag::Prologue)
		{}

		bool PropagateConst() override { return false; }
		void DeadCodeElimination(bool) override { m_IsUsed = true; }//TODO: could remoe funtion prologe when returning const value} // Purposefully empty
		void GenerateASM(std::ostream& o) override;
	};

	class Return : public Instruction
	{
	public:
		// Return param can be either a symbol name or a value (or empty when returning nothing)
		Return(std::string returnParam, Scope* scope)
			: Return(scope->GetSymbol(returnParam), scope)
		{}

		Return(Symbol* returnParam, Scope* scope)
			: Instruction(scope, InstructionTag::Return), m_ReturnParam(returnParam)
		{}

		bool PropagateConst() override;
		void GenerateASM(std::ostream& o) override;
		void DeadCodeElimination(bool allowSetUnused) override;

	private:
		Symbol* m_ReturnParam;
		std::optional<int> m_ConstVal{}; // Used in case the return value is const so we can save that value without it changing
		//std::string m_ReturnParam;
	};

	class Call : public Instruction
	{
	public:
		Call(const std::string& tempResultName, std::string uniqueFuncName, size_t amountOfParams, Scope* scope)
			: Call(scope->GetSymbol(tempResultName), std::move(uniqueFuncName), amountOfParams, scope)
		{}
		Call(const Symbol* param, std::string uniqueFuncName, size_t amountOfParams, Scope* scope)
			: Instruction(scope, InstructionTag::Call)
			, m_ResultSym(param)
			, m_UniqueFuncName(std::move(uniqueFuncName))
			, m_AmountOfParams(amountOfParams)
		{}

		bool PropagateConst() override { return false; }
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override;

	private:
		const Symbol* m_ResultSym;
		std::string m_UniqueFuncName;
		size_t m_AmountOfParams;
	};

	class WriteParam : public Instruction
	{
	public:
		WriteParam(const std::string& paramName, const size_t paramIdx, const std::string& funcName, Scope* scope)
			: WriteParam(scope->GetSymbol(paramName), paramIdx, scope->GetFunc(funcName) ,scope)
		{}
		WriteParam(Symbol* sym, const size_t paramIdx, Function* func, Scope* scope)
			: Instruction(scope, InstructionTag::WriteParam)
			, m_Sym(sym)
			, m_ParamIdx(paramIdx)
			, m_Function(func)
		{}

		bool PropagateConst() override;
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override;

	private:
		Symbol* m_Sym;
		const size_t m_ParamIdx;
		const Function* m_Function;
		std::optional<int> m_ConstVal{}; // Used in case the return value is const so we can save that value without it changing
	};

	class ReadParam : public Instruction
	{
	public:
		ReadParam(const std::string& paramName, const size_t paramIdx, Scope* scope)
			: ReadParam(scope->GetSymbol(paramName), paramIdx, scope)
		{}
		ReadParam(Symbol* sym, const size_t paramIdx, Scope* scope)
			: Instruction(scope, InstructionTag::WriteParam)
			, m_Sym(sym)
			, m_ParamIdx(paramIdx)
		{}

		bool PropagateConst() override { return false; }
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override;

	private:
		Symbol* m_Sym;
		const size_t m_ParamIdx;
	};
#pragma endregion FunctionOperations

	class WriteConst : public Instruction
	{
	public:
		WriteConst(const std::string& symName, std::string valueSting, Scope* scope)
			: WriteConst(scope->GetSymbol(symName), std::move(valueSting), scope)
		{}
		WriteConst(Symbol* sym, std::string valueSting, Scope* scope)
			: Instruction(scope, InstructionTag::WriteConst)
			, m_Sym(sym)
			, m_ValueString(std::move(valueSting))
		{}

		bool PropagateConst() override;
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override;
		void SetUsed(bool used) { m_IsConstUsed = used; }

	private:
		Symbol* m_Sym;
		std::string m_ValueString;
		bool m_IsConstUsed{false};
	};

	class Assign : public Instruction
	{
	public:
		Assign(const std::string& destSymName, const std::string& sourceSymName, Scope* scope)
			: Assign(scope->GetSymbol(destSymName), scope->GetSymbol(sourceSymName), scope)
		{}
		Assign(Symbol* destSym, Symbol* sourceSym, Scope* scope)
			: Instruction(scope, InstructionTag::Assign)
			, m_DestSym(destSym)
			, m_SourceSym(sourceSym)
		{}

		~Assign() override { std::cout << "Destructor called"; }

		bool PropagateConst() override;
		void DeadCodeElimination(bool allowSetUnused) override;
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
		Declaration(Symbol* sym, Scope* scope)
			: Instruction(scope, InstructionTag::Declaration)
			, m_Sym(sym)
		{}

		bool PropagateConst() override;
		// Purposefully empty
		void DeadCodeElimination(bool allowSetUnused) override { m_IsUsed = true; }
		// Purposefully empty
		void GenerateASM(std::ostream& o) override {}

	private:
		Symbol* m_Sym;
	};

#pragma region UnaryOperation
	class UnaryOperation : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		UnaryOperation(const std::string& tempSymName, const std::string& origSymName, Scope* scope)
			: UnaryOperation(scope->GetSymbol(tempSymName), scope->GetSymbol(origSymName), scope)
		{}
		UnaryOperation(Symbol* tempSym, Symbol* origSym, Scope* scope)
			: Instruction(scope, InstructionTag::UnaryOperation)
			, m_TempSym(tempSym)
			, m_OrigSym(origSym)
		{}

		bool PropagateOneOperantConst(const std::function<int(int)>& operation);
	protected:
		Symbol* m_TempSym;
		Symbol* m_OrigSym;
	};

	class Negate : public UnaryOperation
	{
	public:
		Negate(const std::string& tempSymName, const std::string& origSymName, Scope* scope)
			: Negate(scope->GetSymbol(tempSymName), scope->GetSymbol(origSymName), scope)
		{}
		Negate(Symbol* tempSym, Symbol* origSym, Scope* scope)
			: UnaryOperation(tempSym, origSym, scope)
		{}

		bool PropagateConst() override { return PropagateOneOperantConst(std::negate<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class Not : public UnaryOperation
	{
	public:
		Not(const std::string& tempSymName, const std::string& origSymName, Scope* scope)
			: Not(scope->GetSymbol(tempSymName), scope->GetSymbol(origSymName), scope)
		{}
		Not(Symbol* tempSym, Symbol* origSym, Scope* scope)
			: UnaryOperation(tempSym, origSym, scope)
		{}

		bool PropagateConst() override { return PropagateOneOperantConst(std::logical_not<>()); }
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion UnaryOperation

#pragma region BinaryOperation
	class BinaryOperation : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		BinaryOperation(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope)
		{}
		BinaryOperation(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Instruction(scope, InstructionTag::BinaryOperation)
			, m_ResultSym(resultSym)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		virtual bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) = 0;
		bool PropagateBinaryConst(const std::function<int(int, int)>& operation);
	protected:
		Symbol* m_ResultSym;
		Symbol* m_LhsSym;
		Symbol* m_RhsSym;
		// If there's only one const value we cant do constant folding but we can prevent a loading of that variable
		// by just writng the const
		std::optional<int> m_ConstVal{};
		bool m_LhsIsConst = false;
	};

#pragma region Additive
	//class Additive : public Instruction
	//{
	//public:
	//	bool PropagateConst() override = 0;
	//	void GenerateASM(std::ostream& o) override = 0;
	//
	//protected:
	//	Additive(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, InstructionTag op)
	//		: Additive(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
	//	{}
	//	Additive(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, InstructionTag op)
	//		: Instruction(scope, op)
	//		, m_ResultSym(resultSym)
	//		, m_LhsSym(lhsSym)
	//		, m_RhsSym(rhsSym)
	//	{}
	//
	//	bool CheckObsoleteExpr();
	//
	//	Symbol* m_ResultSym;
	//	const Symbol* m_LhsSym;
	//	const Symbol* m_RhsSym;
	//	// If there's only one const value we cant do constant folding but we can prevent a loading of that variable
	//	// by just writng the const
	//	std::optional<int> m_ConstVal{};
	//	bool m_LhsIsConst = false;
	//
	//};

	class Plus : public BinaryOperation
	{
	public:
		Plus(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Plus(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::plus<>()); }
		void GenerateASM(std::ostream& o) override;

	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class Minus : public BinaryOperation
	{
	public:
		Minus(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Minus(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::minus<>()); }
		void GenerateASM(std::ostream& o) override;

	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

#pragma endregion Additive

#pragma region Multiplicative
	//class Multiplicative : public Instruction
	//{
	//public:
	//	bool PropagateConst() override = 0;
	//	void GenerateASM(std::ostream& o) override = 0;
	//
	//protected:
	//	Multiplicative(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, InstructionTag op)
	//		: Multiplicative(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
	//	{}
	//	Multiplicative(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, InstructionTag op)
	//		: Instruction(scope, op)
	//		, m_ResultSym(resultSym)
	//		, m_LhsSym(lhsSym)
	//		, m_RhsSym(rhsSym)
	//	{}
	//
	//	Symbol* m_ResultSym;
	//	const Symbol* m_LhsSym;
	//	const Symbol* m_RhsSym;
	//
	//	std::optional<int> m_ConstVal{};
	//	bool m_LhsIsConst = false;
	//};

	class Mul : public BinaryOperation
	{
	public:
		Mul(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Mul(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::multiplies<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class Div : public BinaryOperation
	{
	public:
		Div(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Div(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::divides<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class Mod : public BinaryOperation
	{
	public:
		Mod(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Mod(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::modulus<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};
#pragma endregion Multiplicative

#pragma region Bitwise
	//class Bitwise : public Instruction
	//{
	//public:
	//	bool PropagateConst() override = 0;
	//	void GenerateASM(std::ostream& o) override = 0;
	//
	//protected:
	//	Bitwise(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, InstructionTag op)
	//		: Bitwise(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
	//	{}
	//	Bitwise(Symbol* resultSym, const Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, InstructionTag op)
	//		: Instruction(scope, op)
	//		, m_ResultSym(resultSym)
	//		, m_LhsSym(lhsSym)
	//		, m_RhsSym(rhsSym)
	//	{}
	//
	//	Symbol* m_ResultSym;
	//	const Symbol* m_LhsSym;
	//	const Symbol* m_RhsSym;
	//
	//	std::optional<int> m_ConstVal{};
	//	bool m_LhsIsConst = false;
	//};

	class BitwiseAnd : public BinaryOperation
	{
	public:
		BitwiseAnd(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		BitwiseAnd(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::bit_and<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class BitwiseOr : public BinaryOperation
	{
	public:
		BitwiseOr(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		BitwiseOr(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::bit_or<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class BitwiseXor : public BinaryOperation
	{
	public:
		BitwiseXor(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: BinaryOperation(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		BitwiseXor(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::bit_xor<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};
#pragma endregion Bitwise

#pragma region Comparison
	class Comparison : public BinaryOperation
	{
	public:
		bool PropagateConst() override = 0;
		void GenerateASM(std::ostream& o) override = 0;
	
	protected:
		Comparison(const std::string& resultSymName, const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: Comparison(scope->GetSymbol(resultSymName), scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope)
		{}
		Comparison(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: BinaryOperation(resultSym, lhsSym, rhsSym, scope)
		{}
	
		void GenerateCmpASM(std::ostream& o, std::string cmpOpInstr, const std::string& OpName);
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override { return false; } // Purposefully "empty"
	};

	class LessThan : public Comparison
	{
	public:
		LessThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		LessThan(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::less<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class GreaterThan : public Comparison
	{
	public:
		GreaterThan(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		GreaterThan(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::greater<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class Equal : public Comparison
	{
	public:
		Equal(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		Equal(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::equal_to<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class NotEqual : public Comparison
	{
	public:
		NotEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		NotEqual(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::not_equal_to<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class LessOrEqual : public Comparison
	{
	public:
		LessOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		LessOrEqual(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::less_equal<>()); }
		void GenerateASM(std::ostream& o) override;
	};

	class GreaterOrEqual : public Comparison
	{
	public:
		GreaterOrEqual(std::string resultSymName, std::string lhsSymName, std::string rhsSymName, Scope* scope)
			: Comparison(resultSymName, lhsSymName, rhsSymName, scope)
		{}
		GreaterOrEqual(Symbol* resultSym, Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Comparison(resultSym, lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateBinaryConst(std::greater_equal<>()); }
		void GenerateASM(std::ostream& o) override;
	};
#pragma endregion Comparison
#pragma endregion BinaryOperation

#pragma region CompoundAssignment
	class CompoundAssignment : public Instruction
	{
	public:
		bool PropagateConst() override = 0;
		void DeadCodeElimination(bool allowSetUnused) override;
		void GenerateASM(std::ostream& o) override = 0;

	protected:
		CompoundAssignment(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: CompoundAssignment(scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope)
		{}
		CompoundAssignment(Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: Instruction(scope, InstructionTag::CompoundAssignment)
			, m_LhsSym(lhsSym)
			, m_RhsSym(rhsSym)
		{}

		virtual bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) = 0;
		bool PropagateCompoundConst(const std::function<int(int, int)>& operation);

		Symbol* m_LhsSym;
		Symbol* m_RhsSym;

		// If there's only one const value we cant do constant folding but we can prevent a loading of that variable
		// by just writng the const
		std::optional<int> m_ConstVal{};
		bool m_LhsIsConst = false;
	};

#pragma region AdditiveEqual
	//class AdditiveEqual : public Instruction
	//{
	//public:
	//	bool PropagateConst() override = 0;
	//	void GenerateASM(std::ostream& o) override = 0;
	//
	//protected:
	//	AdditiveEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, InstructionTag op)
	//		: AdditiveEqual(scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
	//	{}
	//	AdditiveEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, InstructionTag op)
	//		: Instruction(scope, op)
	//		, m_LhsSym(lhsSym)
	//		, m_RhsSym(rhsSym)
	//	{}
	//
	//	Symbol* m_LhsSym;
	//	const Symbol* m_RhsSym;
	//
	//	std::optional<int> m_ConstVal{};
	//};

	class PlusEqual : public CompoundAssignment
	{
	public:
		PlusEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: CompoundAssignment(lhsSymName, rhsSymName, scope)
		{}
		PlusEqual(Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: CompoundAssignment(lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateCompoundConst(std::plus<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class MinusEqual : public CompoundAssignment
	{
	public:
		MinusEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: CompoundAssignment(lhsSymName, rhsSymName, scope)
		{}
		MinusEqual(Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: CompoundAssignment(lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateCompoundConst(std::minus<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};
#pragma endregion AdditiveEqual

#pragma region MultiplicativeEqual
	//class MultiplicativeEqual : public Instruction
	//{
	//public:
	//	bool PropagateConst() override = 0;
	//	void GenerateASM(std::ostream& o) override = 0;
	//
	//protected:
	//	MultiplicativeEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope, InstructionTag op)
	//		: MultiplicativeEqual(scope->GetSymbol(lhsSymName), scope->GetSymbol(rhsSymName), scope, op)
	//	{}
	//	MultiplicativeEqual(Symbol* lhsSym, const Symbol* rhsSym, Scope* scope, InstructionTag op)
	//		: Instruction(scope, op)
	//		, m_LhsSym(lhsSym)
	//		, m_RhsSym(rhsSym)
	//	{}
	//
	//	Symbol* m_LhsSym;
	//	const Symbol* m_RhsSym;
	//
	//	std::optional<int> m_ConstVal{};
	//};

	class MulEqual : public CompoundAssignment
	{
	public:
		MulEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: CompoundAssignment(lhsSymName, rhsSymName, scope)
		{}
		MulEqual(Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: CompoundAssignment(lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateCompoundConst(std::multiplies<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};

	class DivEqual : public CompoundAssignment
	{
	public:
		DivEqual(const std::string& lhsSymName, const std::string& rhsSymName, Scope* scope)
			: CompoundAssignment(lhsSymName, rhsSymName, scope)
		{}
		DivEqual(Symbol* lhsSym, Symbol* rhsSym, Scope* scope)
			: CompoundAssignment(lhsSym, rhsSym, scope)
		{}

		bool PropagateConst() override { return PropagateCompoundConst(std::divides<>()); }
		void GenerateASM(std::ostream& o) override;
	protected:
		bool CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst) override;
	};
#pragma endregion MultiplicativeEqual
#pragma endregion CompoundAssignment

	class ConditionalJump : public Instruction
	{
	public:
		ConditionalJump(const std::string& conditionSymName, std::string exitTrueLabel, std::string exitFalseLabel, Scope* scope)
			: ConditionalJump(scope->GetSymbol(conditionSymName), std::move(exitTrueLabel), std::move(exitFalseLabel), scope)
		{}
		ConditionalJump(Symbol* conditionSym, std::string exitTrueLabel, std::string exitFalseLabel, Scope* scope)
			: Instruction(scope, InstructionTag::ConditionalJump)
			, m_ConditionSym(conditionSym)
			, m_ExitTrueLabel(std::move(exitTrueLabel))
			, m_ExitFalseLabel(std::move(exitFalseLabel))
		{}

		bool PropagateConst() override;
		void DeadCodeElimination(bool ) override;
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
			: Instruction(scope, InstructionTag::UnconditionalJump)
			, m_ExitTrueLabel(std::move(exitTrueLabel))
		{}

		bool PropagateConst() override;
		void DeadCodeElimination(bool ) override { m_IsUsed = true; } // Purposefully empty
		void GenerateASM(std::ostream& o) override;

	private:
		const std::string m_ExitTrueLabel;
	};

}
