#include "Instruction.h"
#include "BasicBlock.h"
#include "../SymbolTable.h"
#include "../ErrorHandeler.h"
#include <cassert>
#include <utility>
#include <format>
#include <iostream>
#include "ControlFlowGraph.h"

//https://www.cs.virginia.edu/~evans/cs216/guides/x86.html

#pragma region FunctionOperations
// This prologue sets up the stack frame for the function, which will be used to access local
// variables and pass arguments to other functions. It also saves the old base pointer, which
// will be used to restore the previous stack frame when the function returns.
void Operation::Prologue::GenerateASM(std::ostream& o)
{
	const std::string label = m_BasicBlock->GetLabel();

	// .globl my_function, declares my_function as a global symbol, meaning it can be called from other files.
	o << ".globl " << label << '\n';
	// '.type' directive is used to specify the type of a symbol '@function' Indicates that the symbol is a function.
	//Little confused about the .type but it gives errors when trying to compile on windows and commenting it fixes it
	//Possible explanation https://sourceware.org/binutils/docs/as/Type.html#Type
	//o << ".type " << label << ", @function" << '\n';
	// 'my_function:' labels the start of the function.
	o << label << ":" << '\n';

	//if (!m_BasicBlock->GetFunction()->isConst)
	{
		o << FormatInstruction("pushq", "%rbp"); AddCommentToPrevInstruction(o, "[Prologue] Save the old base pointer");
		o << FormatInstruction("movq", "%rsp", "%rbp"); AddCommentToPrevInstruction(o, "[Prologue] Set the base pointer to the current stack pointer");
	}

	// Keep the size a multiple of 16 for mem alignment reasons
	const int size = RoundUpToMultipleOf16(m_Scope->GetScopeSize());
	if (m_BasicBlock->GetFunction()->hasFuncCall && size > 0)
	{
		o << FormatInstruction("subq", "$" + std::to_string(size), "%rsp"); AddCommentToPrevInstruction(o, "[Prologue] Make room on the stack for local variables");
	}
}

bool Operation::Return::PropagateConst()
{
	// Check if its returning a symbol
	if (m_ReturnParam != nullptr)
	{
		if (m_BasicBlock->GetConst(m_ReturnParam).has_value())
		{
			m_BasicBlock->GetFunction()->isConst = true;
			m_ConstVal = m_ReturnParam->constVal;
		}
	}

	return false;
}

void Operation::Return::DeadCodeElimination(bool allowSetUnused)
{
	// Check if its returning a symbol
	if (!m_ConstVal.has_value())
	{
		m_ReturnParam->isUsed = true;
	}
	m_IsUsed = true;
}

//https://scottc130.medium.com/implementing-functions-in-x86-assembly-a2fb7315e2e0
void Operation::Return::GenerateASM(std::ostream& o) //TODO: if the return of a function is a constptr then we can replace the whole function by a return of that value
{
	if (m_ReturnParam != nullptr)
	{
		if (m_ConstVal.has_value())
		{
			// Returning a const
			o << FormatInstruction("movl", "$" + std::to_string(m_ConstVal.value()), "%eax");
			AddCommentToPrevInstruction(o, "[Return] save return value in the result adress");
		}
		else
		{
			// Returning a variable
			o << FormatInstruction("movl", m_ReturnParam->GetOffsetReg(), "%eax");
			AddCommentToPrevInstruction(o, "[Return] save " + m_ReturnParam->varName + " in the result adress");
		}

	}

	//if(!m_BasicBlock->GetFunction()->isConst)
	{
		// Move stack pointer back to where it was before the function
		o << FormatInstruction("movq", "%rbp", "%rsp"); AddCommentToPrevInstruction(o, "[Return] Move stack pointer back to where it was before the function");
		//https://stackoverflow.com/questions/4584089/what-is-the-function-of-the-push-pop-instructions-used-on-registers-in-x86-ass
		// Move base pointer back to where it was before the function
		o << FormatInstruction("popq", "%rbp"); AddCommentToPrevInstruction(o, "[Return] Retrieve base pointer");
	}

	o << FormatInstruction("ret"); AddCommentToPrevInstruction(o, "[Return]");
}

void Operation::Call::DeadCodeElimination(bool allowSetUnused)
{
	if (m_ResultSym && !m_ResultSym->isUsed)
	{
		m_ResultSym = nullptr;
	}

	//TODO: check when we can delete the whole function call
	//NOTE: you cant delete a function if something like printing happens in the function
	m_IsUsed = true;
}

void Operation::Call::GenerateASM(std::ostream& o)
{
	o << FormatInstruction("call", m_UniqueFuncName); AddCommentToPrevInstruction(o, "[Call]");
	if (m_AmountOfParams > 6) //TODO: test this properly!
	{
		// *8 because we use 'addq' in writeParam so we push 64bits at a time (and we do addq bc we add to RSP which is 64bit)
		o << FormatInstruction("addq", '$' + std::to_string((m_AmountOfParams - m_AmountOfRegisters) * 8), "%rsp"); AddCommentToPrevInstruction(o, "[Call] Move SP back to position where it was before pushing the params that didn't fit in registers");
	}

	//NOTE: only needed when function actually returns something
	//save function return value
	if (m_ResultSym != nullptr)
	{
		o << FormatInstruction("movl", "%eax", m_ResultSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[Call] Save Func Return value into " + m_ResultSym->varName);
	}
}

bool Operation::WriteParam::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_Sym).has_value())
	{
		m_ConstVal = m_Sym->constVal;
	}

	return false;
}

void Operation::WriteParam::DeadCodeElimination(bool allowSetUnused)
{
	if(!m_Function->isCalled)
		return;

	
	if (m_Function->scope->GetSymbol(m_Function->parameterNames[m_ParamIdx])->isUsed)
	{
		m_IsUsed = true;
		m_Sym->isUsed = true;
	}
}

void Operation::WriteParam::GenerateASM(std::ostream& o)
{
	// Use registers as long as there are available
	if (m_ParamIdx < m_AmountOfRegisters)
	{
		//https://stackoverflow.com/questions/5134614/const-map-element-access
		const std::string reg = m_TypeRegisterMap.at(m_Sym->varType)[m_ParamIdx];
		const std::string movInstr = (m_Sym->varType == "char") ? "movb" : "movl";

		std::string param;
		if (m_ConstVal)
			param = '$' + std::to_string(*m_ConstVal);
		else
			param = m_Sym->GetOffsetReg();

		//TODO: Test this for char's
		// Move parameters in to registers
		o << FormatInstruction(movInstr, param, reg); AddCommentToPrevInstruction(o, "[WriteParam] move param:" + m_Sym->varName + " into " + reg);
	}
	// If we ran out of registers push parameters on the stack
	else
	{
		//NOTE: THESE PUSH'S DON'T HAVE ANY POP'S
		//2nd NOTE: (they don't need any as they will be overwritten by next function as the sp moves behind them (See: Instruction::Call))
		if (m_ConstVal)
		{
			o << FormatInstruction("pushq", '$' + std::to_string(*m_ConstVal));
		}
		else
		{
			if (m_Sym->varType == "char")
			{
				o << FormatInstruction("movzbl", m_Sym->GetOffsetReg(), "%eax");
				o << FormatInstruction("pushq", "%rax");
			}
			else
			{
				o << FormatInstruction("pushq", m_Sym->GetOffsetReg());
			}

		}
		AddCommentToPrevInstruction(o, "[WriteParam] Push parameter on stack as theres no free register left");
	}
}

void Operation::ReadParam::DeadCodeElimination(bool allowSetUnused)
{
	if (m_Sym->isUsed)
	{
		m_IsUsed = true;
	}
}

void Operation::ReadParam::GenerateASM(std::ostream& o)
{
	// Use registers as long as there are available
	if (m_ParamIdx < m_AmountOfRegisters) {

		//https://stackoverflow.com/questions/5134614/const-map-element-access
		const std::string reg = m_TypeRegisterMap.at(m_Sym->varType)[m_ParamIdx];
		const std::string movInstr = (m_Sym->varType == "char") ? "movb" : "movl";

		o << FormatInstruction(movInstr, reg, m_Sym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[ReadParam] move " + reg + " into param:" + m_Sym->varName);
	}
	// No registers for the others so they have to be read by their offset from the base pointer
}
#pragma endregion FunctionOperations

bool Operation::WriteConst::PropagateConst()
{
	//TODO: remove code in visitor add code here dont forget to delete temp var
	return false;
}

void Operation::WriteConst::DeadCodeElimination(bool allowSetUnused)
{
	if (m_Sym->isUsed && m_IsConstUsed)
	{
		m_Sym->isUsed = !allowSetUnused;
		//TODO: delete all write consts with the other flag first or check that flag here
		m_IsUsed = true; //TODO: set this variable in every function and don't forget the purposefully empty once's

	}
}

void Operation::WriteConst::GenerateASM(std::ostream& o)
{
	//if constexpr (g_RemoveDeadcode)
	//{
	//	if (!m_IsConstUsed)
	//		return;
	//}

	const bool isChar = m_Sym->varType == "char";

	o << FormatInstruction(isChar ? "movb" : "movl", "$" + m_ValueString, m_Sym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[WriteConst] move " + m_ValueString + " into " + m_Sym->varName);

}

bool Operation::Assign::PropagateConst()
{
	if(m_BasicBlock->GetConst(m_SourceSym).has_value())
	{
		//if (m_DestSym->isUsed) //TODO: do this better there has to be a better way for unused variables
		//{
			//m_DestSym->constVal = m_SourceSym->constVal;
			auto* constInstr = new Operation::WriteConst(m_DestSym, std::to_string(*m_SourceSym->constVal), m_Scope);
			m_BasicBlock->AddAndUpdateConst(m_DestSym, m_SourceSym->constVal, constInstr);
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//}
		//else
		//	return true; // TODO: also delete the variable at some point
	}
	else
	{
		//m_DestSym->constVal = std::nullopt;
		m_BasicBlock->AddAndUpdateConst(m_DestSym, std::nullopt, {});
	}

	return false;
}

void Operation::Assign::DeadCodeElimination(bool allowSetUnused)
{
	if (m_DestSym->isUsed)
	{
		//set to false if allowed otherswise leave it true
		m_DestSym->isUsed = !allowSetUnused;

		m_SourceSym->isUsed = true;
		m_IsUsed = true;
	}
}

void Operation::Assign::GenerateASM(std::ostream& o)
{
	const std::pair<std::string, std::string> instPair = GetMoveInstr(m_DestSym->varType, m_SourceSym->varType);
	const std::string reg = m_DestSym->varType == "int" ? "%eax" : "%al";

	o << FormatInstruction(instPair.first, m_SourceSym->GetOffsetReg(), reg); AddCommentToPrevInstruction(o, "[Assign] move " + m_SourceSym->varName + " in accumulator");
	o << FormatInstruction(instPair.second, reg, m_DestSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[Assign] move accumulator in " + m_DestSym->varName);
}

bool Operation::Declaration::PropagateConst()
{
	m_BasicBlock->AddAndUpdateConst(m_Sym, std::nullopt, {});
	return true;
}

#pragma region Unary
bool Operation::UnaryOperation::PropagateOneOperantConst(const std::function<int(int)>& operation)
{
	if (m_BasicBlock->GetConst(m_OrigSym).has_value())
	{
		const int res = operation(m_OrigSym->constVal.value());
		m_TempSym->constVal = res;
		//TODO: maybe also delete origSym (will always be temp and set in write const) If we want to delete it add a delete sym to our scope
		return true;
	}
	return false;
}

void Operation::UnaryOperation::DeadCodeElimination(bool allowSetUnused)
{
	if (m_TempSym->isUsed)
	{
		m_OrigSym->isUsed = true;
		m_IsUsed = true;
	}
}

void Operation::Negate::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_OrigSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr2;
	std::string reg2;
	if (m_TempSym->varType == "char")
	{
		reg2 = "%al";
		movInstr2 = "movb";
	}
	else
	{
		reg2 = "%eax";
		movInstr2 = "movl";
	}

	o << FormatInstruction(movInstr1, m_OrigSym->GetOffsetReg(), "%eax"); AddCommentToPrevInstruction(o, "[Negate] copy original value to the accumulator");
	o << FormatInstruction("negl", "%eax"); AddCommentToPrevInstruction(o, "[Negate] negate the accumulator");
	o << FormatInstruction(movInstr2, reg2, m_TempSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[Negate] save negated value in " + m_TempSym->varName);
}

void Operation::Not::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_OrigSym->varType == "char" ? "movsbl" : "movl";

	o << FormatInstruction(movInstr1, m_OrigSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Not] copy original value to the accumulator");
	o << FormatInstruction("cmpl", "$0", "%eax");								AddCommentToPrevInstruction(o, "[Not] compaire value to 0");
	o << FormatInstruction("sete", "%al");											AddCommentToPrevInstruction(o, "[Not] set flag equal");
	if (m_TempSym->varType == "char")
	{
		o << FormatInstruction("movb", "%al", m_TempSym->GetOffsetReg());		AddCommentToPrevInstruction(o, "[Not] save result into " + m_TempSym->varName);
	}
	else
	{
		o << FormatInstruction("movzbl", "%al", "%eax");						AddCommentToPrevInstruction(o, "[Not] save result into EAX");
		o << FormatInstruction("movl", "%eax", m_TempSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Not] save EAX " + m_TempSym->varName);
	}
}
#pragma endregion Unary

#pragma region TwoOperandInstructions
bool Operation::BinaryOperation::PropagateBinaryConst(const std::function<int(int, int)>& operation)
{
	const bool lhsIsConst = m_BasicBlock->GetConst(m_LhsSym).has_value();
	const bool rhsIsConst = m_BasicBlock->GetConst(m_RhsSym).has_value();

	// If both are const calculate the result during compile time
	if (lhsIsConst && rhsIsConst)
	{
		// Calculate the result so we can update the constval
		const int res = operation(m_LhsSym->constVal.value(), m_RhsSym->constVal.value());
		m_ResultSym->constVal = res;

		// Return true to indicate that we can delete this instruction (as its already calculated at compile time so no need to write assembly for it)
		return true;
	}

	// If only one of them is cost mark that one as const so we know to use the constval later
	if (lhsIsConst)
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;
	}
	else if (rhsIsConst)
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;
	}

	// Check for obsolete expressions like "x + 0" that do nothing
	// or "x * 0" by which we know the value of x at compile time and thus can delete the instruction (by returning true)
	return CheckObsoleteExpr(lhsIsConst, rhsIsConst);
}

void Operation::BinaryOperation::DeadCodeElimination(bool allowSetUnused)
{
	if (m_ResultSym->isUsed)
	{
		m_LhsSym->isUsed = true;
		m_RhsSym->isUsed = true;
		m_IsUsed = true;
	}
}

#pragma region Additive
bool Operation::Plus::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression? (eg. x + 0 || x - 0 || 0 + x || etc...)
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_RhsSym;
		return true;
	}
	else if (rhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}
	return false;
}

std::pair<std::string, std::string> InstructionRegisterAlloc(Symbol* lhs, Symbol* rhs, bool lhsIsConst, std::optional<int> constval)
{
	
}

void Operation::Plus::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");							AddCommentToPrevInstruction(o, "[Plus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");							AddCommentToPrevInstruction(o, "[Plus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");					AddCommentToPrevInstruction(o, "[Plus] add values together");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Plus] save result into " + m_ResultSym->varName);
}

bool Operation::Minus::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression? (eg. x + 0 || x - 0 || 0 + x || etc...)
	// 0 - x = -x
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		m_BasicBlock->ReplaceInstruction(this, new Negate(m_ResultSym, m_RhsSym, m_Scope));
	}
	if (rhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}
	return false;
}

void Operation::Minus::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[Minus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[Minus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[Minus] subtract values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Minus] save result into " + m_ResultSym->varName);
}
#pragma endregion Additive

#pragma region Multiplicative
void Operation::Mul::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");									AddCommentToPrevInstruction(o, "[Mul] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[Mul] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[Mul] multiply values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Mul] save result into " + m_ResultSym->varName);
}

bool Operation::Mul::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression? (eg. x * 1 || 1 * x || 0 * x || ...)
	if (lhsIsConst)
	{
		if (m_ConstVal == 1)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
		else if (m_ConstVal == 0)
		{
			m_ResultSym->constVal = 0;
			return true;
		}
	}
	else if (rhsIsConst)
	{
		if (m_ConstVal == 1)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
		else if (m_ConstVal == 0)
		{
			m_ResultSym->constVal = 0;
			return true;
		}
	}
	return false;
}

void Operation::Div::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();

	o << FormatInstruction(movInstr1, lhs, "%eax");									AddCommentToPrevInstruction(o, "[Div] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", rhs, "%ecx");						AddCommentToPrevInstruction(o, "[Div] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Div] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");										AddCommentToPrevInstruction(o, "[Div] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Div] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", rhs);										AddCommentToPrevInstruction(o, "[Div] divide and store result in to EDX:EAX");
	}


	if (m_ResultSym->varType == "char")
	{
		o << FormatInstruction("movb", "%al", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Div] save result into " + m_ResultSym->varName);
	}
	else
	{
		o << FormatInstruction("movl", "%eax", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Div] save result into " + m_ResultSym->varName);
	}
}

bool Operation::Div::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression? (eg. x / 1 || 0 / x )
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_ConstVal == 0)
	{
		m_ResultSym->constVal = 0;
		return true;
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value() && m_RhsSym->constVal.value() == 1)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}

	return false;
}

void Operation::Mod::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();

	o << FormatInstruction(movInstr1, lhs, "%eax");				AddCommentToPrevInstruction(o, "[Mod] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", rhs, "%ecx");	AddCommentToPrevInstruction(o, "[Mod] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");								AddCommentToPrevInstruction(o, "[Mod] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");					AddCommentToPrevInstruction(o, "[Mod] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");								AddCommentToPrevInstruction(o, "[Mod] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", rhs);					AddCommentToPrevInstruction(o, "[Mod] divide and store result in to EDX:EAX");
	}


	if (m_ResultSym->varType == "char")
	{
		o << FormatInstruction("movl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[Mod] mov result in to EAX so we can do an movb operation");
		o << FormatInstruction("movb", "%al", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Mod] save result into " + m_ResultSym->varName);
	}
	else
	{
		o << FormatInstruction("movl", "%edx", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Mod] save result into " + m_ResultSym->varName);
	}
}

bool Operation::Mod::CheckObsoleteExpr(bool lhsIsConst, bool)
{
	// Check obsolete expression? (eg. x % 0)
	if (lhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}

	return false;
}

#pragma endregion Multiplicative

#pragma region BitwiseOperations
void Operation::BitwiseAnd::GenerateASM(std::ostream& o)
{
	//TODO: Could be improved I think same for or and xor
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("andl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseAnd] bitwise AND op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseAnd] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseAnd::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression?
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		m_ResultSym->constVal = 0;
		return true;
	}
	else if (rhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		m_ResultSym->constVal = 0;
		return true;
	}
	return false;
}

void Operation::BitwiseOr::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("orl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseOr] bitwise OR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseOr] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseOr::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression?
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_RhsSym;
		return true;
	}
	else if (rhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}
	return false;
}

void Operation::BitwiseXor::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";
	std::string movInstr3;
	std::string reg3;
	if (m_ResultSym->varType == "char") //TODO: technically useless as where the plus operation is created tempVar is defined as always int
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("xorl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseXor] bitwise XOR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseXor] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseXor::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression?
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_RhsSym;
		return true;
	}
	else if (rhsIsConst && m_RhsSym->constVal.value() == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}
	return false;
}

#pragma endregion BitwiseOperations

#pragma region cmpOperations
void Operation::Comparison::GenerateCmpASM(std::ostream& o, std::string cmpOpInstr, const std::string& OpName)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	//TODO: can improve dont have to load EAX and ADX always
	//TODO: probably best to write a function that takes 2 symbols and does this
	o << FormatInstruction(movInstr1, lhs, "%eax");									AddCommentToPrevInstruction(o, "["+OpName+"] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "["+OpName+"] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("cmpl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "["+OpName+"] compaire values");
	o << FormatInstruction(std::move(cmpOpInstr), "%al");							AddCommentToPrevInstruction(o, "["+OpName+"] set flag "+OpName);
	if (m_ResultSym->varType == "char")
	{
		o << FormatInstruction("movb", "%al", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "["+OpName+"] save result into " + m_ResultSym->varName);
	}
	else
	{
		o << FormatInstruction("movzbl", "%al", "%eax");						AddCommentToPrevInstruction(o, "["+OpName+"] save result into EAX");
		o << FormatInstruction("movl", "%eax", m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "["+OpName+"] save EAX " + m_ResultSym->varName);
	}
}
//TODO: this idea could be used in other propagate consts like addition for plus and min for example
//bool Operation::Comparison::PropagateCmpConst(const std::function<bool(bool, bool)>& operation)
//{
//	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
//	{
//		const int res = operation(m_LhsSym->constVal.value(), m_RhsSym->constVal.value());
//		m_ResultSym->constVal = res;
//		return true;
//	}
//
//	if (m_BasicBlock->GetConst(m_LhsSym).has_value())
//	{
//		m_ConstVal = m_LhsSym->constVal;
//		m_LhsIsConst = true;
//	}
//	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
//	{
//		m_ConstVal = m_RhsSym->constVal;
//		m_LhsIsConst = false;
//	}
//	return false;
//}

void Operation::LessThan::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setl", "LessThan");
}

void Operation::GreaterThan::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setg", "GreaterThan");
}

void Operation::Equal::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "sete", "Equal");
}

void Operation::NotEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setne", "NotEqual");
}

void Operation::LessOrEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setle", "LessOrEqual");
}

void Operation::GreaterOrEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setge", "GreaterOrEqual");
}


#pragma endregion cmpOperations
#pragma endregion TwoOperandInstructions

#pragma region CompoundStatement
bool Operation::CompoundAssignment::PropagateCompoundConst(const std::function<int(int, int)>& operation)
{
	//TODO: explain properly what this does
	const bool lhsIsConst = m_BasicBlock->GetConst(m_LhsSym).has_value();
	const bool rhsIsConst = m_BasicBlock->GetConst(m_RhsSym).has_value();

	if (lhsIsConst && rhsIsConst)
	{
		//if (m_LhsSym->isUsed)
		//{
			const int res = operation(m_LhsSym->constVal.value(), m_RhsSym->constVal.value());
			//m_LhsSym->constVal = res;
			auto* constInstr = new Operation::WriteConst(m_LhsSym, std::to_string(res), m_Scope);
			m_BasicBlock->AddAndUpdateConst(m_LhsSym, res, constInstr);
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//}
		//else
		//	return true;
	}
	else if (lhsIsConst)
	{
		m_ConstVal = m_LhsSym->constVal;
	}
	else if (rhsIsConst)
	{
		m_ConstVal = m_RhsSym->constVal;
	}

	// Ill admit this is extremely dirty but for now due to a lack of time I wont implement a proper fix
	// The reason why I copy these pointers is because CheckObsoleteExpr might invalidate them by replacing the current instruction
	// with another one and thus I wouldn't be able to set lhs symbol to nullopt using m_LhsSym and m_BasicBlock //TODO: fix :)
	auto* lhsSym = m_LhsSym;
	auto* basicBlock = m_BasicBlock;

	const bool deleteInst = CheckObsoleteExpr(lhsIsConst, rhsIsConst);

	// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
	if (lhsIsConst && !deleteInst) // If we're going to delete this instruction lhs can stay const
	{
		//lhsSym->constVal = std::nullopt;
		basicBlock->AddAndUpdateConst(lhsSym, std::nullopt, {});
	}

	return deleteInst;
}

void Operation::CompoundAssignment::DeadCodeElimination(bool allowSetUnused)
{
	if (m_LhsSym->isUsed)
	{
		m_RhsSym->isUsed = true;
		m_IsUsed = true;
	}
}

#pragma region AdditiveEqual

bool Operation::PlusEqual::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
		m_BasicBlock->ReplaceInstruction(this, new Assign(m_LhsSym, m_RhsSym, m_Scope)); // be aware this is quite dangerous :)
	if (rhsIsConst && m_RhsSym->constVal.value() == 0)
		return true;

	return false;
}

void Operation::PlusEqual::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";

	std::string movInstr3;
	std::string reg3;
	if (m_LhsSym->varType == "char")
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[PlusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[PlusEqual] save result into " + m_LhsSym->varName);
}

bool Operation::MinusEqual::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
		m_BasicBlock->ReplaceInstruction(this, new Negate(m_LhsSym, m_RhsSym, m_Scope)); // be aware this is quite dangerous :)
	if (rhsIsConst && m_RhsSym->constVal.value() == 0)
		return true;

	return false;
}

void Operation::MinusEqual::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";

	std::string movInstr3;
	std::string reg3;
	if (m_LhsSym->varType == "char")
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MinusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MinusEqual] save result into " + m_LhsSym->varName);
}
#pragma endregion AdditiveEqual
#pragma region MultiplicativeEqual

bool Operation::MulEqual::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression?
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
	{
		return true;
	}
	else if (rhsIsConst)
	{
		if (m_RhsSym->constVal.value() == 1)
			return true;
		else if (m_RhsSym->constVal.value() == 0)
		{
			//m_LhsSym->constVal = 0;
			auto* constInstr = new Operation::WriteConst(m_LhsSym, "0", m_Scope);
			m_BasicBlock->AddAndUpdateConst(m_LhsSym, 0, constInstr);
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		}
	}

	return false;
}

void Operation::MulEqual::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";

	std::string movInstr3;
	std::string reg3;
	if (m_LhsSym->varType == "char")
	{
		reg3 = "%al";
		movInstr3 = "movb";
	}
	else
	{
		reg3 = "%eax";
		movInstr3 = "movl";
	}

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[MulEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[MulEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MulEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MulEqual] save result into " + m_LhsSym->varName);
}

bool Operation::DivEqual::CheckObsoleteExpr(bool lhsIsConst, bool rhsIsConst)
{
	// Check obsolete expression?
	if (lhsIsConst && m_LhsSym->constVal.value() == 0)
		return true;
	else if (rhsIsConst && m_RhsSym->constVal.value() == 1)
		return true;

	return false;
}

void Operation::DivEqual::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");				AddCommentToPrevInstruction(o, "[DivEqual] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86

	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", rhs, "%ecx");						AddCommentToPrevInstruction(o, "[DivEqual] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[DivEqual] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");										AddCommentToPrevInstruction(o, "[DivEqual] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[DivEqual] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", rhs);										AddCommentToPrevInstruction(o, "[DivEqual] divide and store result in to EDX:EAX");
	}


	if (m_LhsSym->varType == "char")
	{
		o << FormatInstruction("movb", "%al", m_LhsSym->GetOffsetReg());		AddCommentToPrevInstruction(o, "[DivEqual] save result into " + m_LhsSym->varName);
	}
	else
	{
		o << FormatInstruction("movl", "%eax", m_LhsSym->GetOffsetReg());		AddCommentToPrevInstruction(o, "[DivEqual] save result into " + m_LhsSym->varName);
	}
}

#pragma endregion MultiplicativeEqual
#pragma endregion CompoundStatement

bool Operation::ConditionalJump::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_ConditionSym).has_value())
	{
		if constexpr (g_RemoveConstConditionals)
		{
			if (m_ConditionSym->constVal.value() == 0)
			{
				if (m_BasicBlock->GetExitTrue()->GetEntryBlocks().size() < 2)
					m_BasicBlock->GetCFG()->RemoveBasicBlock(m_ExitTrueLabel);
				else
					m_BasicBlock->GetExitTrue()->RemoveEntryBlock(m_BasicBlock);

				m_BasicBlock->ReplaceInstruction(this, new UnconditionalJump(m_ExitFalseLabel, m_Scope));
			}
			else
			{
				if (m_BasicBlock->GetExitFalse()->GetEntryBlocks().size() < 2)
					m_BasicBlock->GetCFG()->RemoveBasicBlock(m_ExitFalseLabel);
				else
					m_BasicBlock->GetExitFalse()->RemoveEntryBlock(m_BasicBlock);

				m_BasicBlock->ReplaceInstruction(this, new UnconditionalJump(m_ExitTrueLabel, m_Scope));
			}
		}
		/*else 
		{
			m_BasicBlock->AddInstr(new )
		}*/
	}
	return false;
}

void Operation::ConditionalJump::DeadCodeElimination(bool allowSetUnused)
{
	m_IsUsed = true;
	//TODO: maybe check if basic block is not used and then change to unconditional jump
}

void Operation::ConditionalJump::GenerateASM(std::ostream& o)
{
	const std::string cmpInstr = m_ConditionSym->varType == "char" ? "cmpb" : "cmpl";
	o << FormatInstruction(cmpInstr, "$0", m_ConditionSym->GetOffsetReg());		AddCommentToPrevInstruction(o, "[ConditionalJump] check if condition is true or false");
	o << FormatInstruction("je", m_ExitFalseLabel);									AddCommentToPrevInstruction(o, "[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)");
	o << FormatInstruction("jmp", m_ExitTrueLabel);									AddCommentToPrevInstruction(o, "[ConditionalJump] jump inside body");
}

bool Operation::UnconditionalJump::PropagateConst()
{
	return false;
}

void Operation::UnconditionalJump::GenerateASM(std::ostream& o)
{
	o << FormatInstruction("jmp", m_ExitTrueLabel);		AddCommentToPrevInstruction(o, "[UnconditionalJump]");
}


///---------------------------
///	Utils
///---------------------------
#pragma region InstructionUtils
//TODO: remove function as its too confusing
std::pair<std::string, std::string> Instruction::GetMoveInstr(const std::string& dest, const std::string& source)
{
	std::pair<std::string, std::string> instPair{};
	if (dest == "int" && source == "int")
	{
		instPair.first = "movl";
		instPair.second = "movl";
	}
	else if (dest == "char")
	{
		instPair.first = "movb";
		instPair.second = "movb";
	}
	else if (dest == "int" && source == "char")
	{
		instPair.first = "movsbl";
		instPair.second = "movl";
	}

	return instPair;
}
//TODO: rename to "get move inst to 32bit or 8 byte" or something
std::string Instruction::GetMoveInstr(const std::string& source)
{
	if (source == "int")
	{
		return "movl";
	}
	else if(source == "char")
	{
		return "movsbl";
	}

	return "movl";
}

/// formats assembly instruction
/// @returns"	instruction		param1, param2"
std::string Instruction::FormatInstruction(std::string instr, std::string param1, std::string param2)
{
	return std::format("\t\t{}\t{}, {}\n", instr, param1, param2);
}

/// formats assembly instruction
/// @returns"	instruction		param"
std::string Instruction::FormatInstruction(std::string instr, std::string param)
{
	return std::format("\t\t{}\t{}\n", instr, param);
}

std::string Instruction::FormatInstruction(std::string instr)
{
	return std::format("\t\t{}\n", instr);
}

void Instruction::AddCommentToPrevInstruction(std::ostream& o, const std::string& comment)
{
	//remove /n to place comment on same line
	o.seekp(-1, std::ostream::cur);
	o << "\t\t#" + comment + '\n'; //technically overwrite \n
}

int Instruction::RoundUpToMultipleOf16(const int x)
{
	return (x + 15) & ~15;
}
#pragma endregion InstructionUtils
