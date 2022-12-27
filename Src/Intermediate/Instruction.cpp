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
	o << ".type " << label << ", @function" << '\n';
	// 'my_function:' labels the start of the function.
	o << label << ":" << '\n';

	o << FormatInstruction("pushq", "%rbp"); AddCommentToPrevInstruction(o, "[Prologue] Save the old base pointer");
	o << FormatInstruction("movq", "%rsp", "%rbp"); AddCommentToPrevInstruction(o, "[Prologue] Set the base pointer to the current stack pointer");

	// Keep the size a multiple of 16 for mem alignment reasons
	const int size = RoundUpToMultipleOf16(m_Scope->GetScopeSize());
	if (m_BasicBlock->GetFunction()->hasFuncCall && size > 0)
	{
		o << FormatInstruction("subq", "$" + std::to_string(size), "%rsp"); AddCommentToPrevInstruction(o, "[Prologue] Make room on the stack for local variables");
	}
}

//https://scottc130.medium.com/implementing-functions-in-x86-assembly-a2fb7315e2e0
void Operation::Return::GenerateASM(std::ostream& o) //TODO: if the return of a function is a constptr then we can replace the whole function by a return of that value
{
	//check if we're actually returning anything
	if (!m_ReturnParam.empty())
	{
		// Returning a variable
		if (m_Scope->HasSymbol(m_ReturnParam))
		{
			const Symbol* source = m_Scope->GetSymbol(m_ReturnParam);
			const std::string sourceLoc = source->constPtr ? "$" + std::to_string(*source->constPtr) : source->GetOffsetReg();

			o << FormatInstruction("movl", sourceLoc, "%eax");
		}
		// Returning a const value
		else
		{
			// Convert const to right value
			const bool isChar = m_BasicBlock->GetFunction()->returnType == "char";
			const int constValue = isChar ? static_cast<int>(m_ReturnParam[1]) : stoi(m_ReturnParam);

			o << FormatInstruction("movl", "$" + std::to_string(constValue), "%eax");
		}
		AddCommentToPrevInstruction(o, "[Return] save return value in the result adress");
	}

	// Move stack pointer back to where it was before the function
	o << FormatInstruction("movq", "%rbp", "%rsp"); AddCommentToPrevInstruction(o, "[Return] Move stack pointer back to where it was before the function");
	//https://stackoverflow.com/questions/4584089/what-is-the-function-of-the-push-pop-instructions-used-on-registers-in-x86-ass
	// Move base pointer back to where it was before the function
	o << FormatInstruction("popq", "%rbp"); AddCommentToPrevInstruction(o, "[Return] Retrieve base pointer");
	o << FormatInstruction("ret"); AddCommentToPrevInstruction(o, "[Return]");
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
	if (m_BasicBlock->GetFunction()->returnType != "void") //TODO: maybe unessesery as it might be optimized away anyway
	{
		o << FormatInstruction("movl", "%eax", m_Param->GetOffsetReg()); AddCommentToPrevInstruction(o, "[Call] Save Func Return value into " + m_Param->varName);
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
		if (m_Sym->constPtr)
			param = '$' + std::to_string(*m_Sym->constPtr);
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
		if (m_Sym->constPtr)
		{
			o << FormatInstruction("pushq", '$' + std::to_string(*m_Sym->constPtr));
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

void Operation::ReadParam::GenerateASM(std::ostream& o)
{
	// Use registers as long as there are available
	if (m_ParamIdx < m_AmountOfRegisters) {

		//https://stackoverflow.com/questions/5134614/const-map-element-access
		const std::string reg = m_TypeRegisterMap.at(m_Sym->varType)[m_ParamIdx];
		const std::string movInstr = (m_Sym->varType == "char") ? "movb" : "movl";

		o << FormatInstruction(movInstr, reg, m_Sym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[ReadParam] move " + reg + " into param:" + m_Sym->varName);
	}
	//TODO: cleanup
	// If we ran out of registers don't do anything values are already in the right location
	//else //TODO: maybe only do these when they are actually used in code
	//{ //TODO: this code will def have to change maybe the symbol its offset needs to be changed to new offset below or some optimazation magic
		//TODO: but only one can be moved in EAX and this should only be done when it being used
		//int offset = std::stoi(m_Params.at(2));



		//const bool isChar = localParamSym->varType == "char";
		//const std::string reg = isChar ? "%al" : "%eax";
		//const std::string movInstr = isChar ? "movb" : "movl";
		//
		//o << FormatInstruction(movInstr, std::to_string(offset) + "(%rbp)", reg); AddCommentToPrevInstruction(o, "[ReadParam] move param value into " + reg);
		//o << FormatInstruction(movInstr, reg, localParamSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[ReadParam] move " + reg + " into " + localParamSym->varName);
	//}
}
#pragma endregion FunctionOperations

void Operation::WriteConst::GenerateASM(std::ostream& o)
{
	const bool isChar = m_Sym->varType == "char";

	o << FormatInstruction(isChar ? "movb" : "movl", "$" + m_ValueString, m_Sym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[WriteConst] move " + m_ValueString + " into " + m_Sym->varName);

}

#pragma region BasicOperations
bool Operation::Assign::PropagateConst()
{
	// If it is a scope variable assign with a const
	if (m_SourceSym->constPtr)
	{
		if constexpr (g_ConstPropagationAssignment)
		{
			m_DestSym->constPtr = new int(*m_SourceSym->constPtr);
			return true;
		}
		else
		{
			m_BasicBlock->ReplaceInstruction(this, new Operation::WriteConst(m_DestSym, std::to_string(*m_SourceSym->constPtr), m_Scope));
		}
	}
	// If the destination still has a constPtr but we assign a non const to it then destination should also no longer be const
	else if(m_DestSym->constPtr)
	{
		delete m_DestSym->constPtr;
		m_DestSym->constPtr = nullptr;
	}
	return false;
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
	return true;
}

bool Operation::Negate::PropagateConst()
{
	if (m_OrigSym->constPtr)
	{
		const int res = -(*m_OrigSym->constPtr);
		m_TempSym->constPtr = new int(res);
		//TODO: maybe also delete origSym (will always be temp and set in write const) If we want to delete it add a delete sym to our scope
		return true;
	}
	return false;
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

#pragma region Additive
bool Operation::Additive::CheckObsoleteExpr() const
{
	// Check obsolete expression? (eg. x + 0 || x - 0 || 0 + x || etc...)
	if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
	{
		*m_ResultSym = *m_RhsSym;
		return true;
	}
	else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
	{
		*m_ResultSym = *m_LhsSym;
		return true;
	}
	return false;
}

bool Operation::Plus::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr + *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	else
	{
		return CheckObsoleteExpr();
	}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Plus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[Plus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[Plus] add values together");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Plus] save result into " + m_ResultSym->varName);
}

bool Operation::Minus::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr - *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	else
	{
		return CheckObsoleteExpr();
	}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Minus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[Minus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[Minus] subtract values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Minus] save result into " + m_ResultSym->varName);
}
#pragma endregion Additive
#pragma region Multiplicative
bool Operation::Mul::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr * *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x * 1 || 1 * x || 0 * x || ...)
		if (m_LhsSym->constPtr)
		{
			const int value = *m_LhsSym->constPtr;
			if (value == 1)
			{
				*m_ResultSym = *m_RhsSym;
				return true;
			}
			else if(value == 0)
			{
				m_ResultSym->constPtr = new int(0);
				return true;
			}
		}
		else if (m_RhsSym->constPtr)
		{
			const int value = *m_RhsSym->constPtr;
			if (value == 1)
			{
				*m_ResultSym = *m_LhsSym;
				return true;
			}
			else if (value == 0)
			{
				m_ResultSym->constPtr = new int(0);
				return true;
			}
		}
	}

	return false;
}

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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Mul] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[Mul] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[Mul] multiply values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Mul] save result into " + m_ResultSym->varName);
}

bool Operation::Div::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr / *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x / 1 || 0 / x )
		if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
		{
			m_ResultSym->constPtr = new int(0);
			return true;
		}
		else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 1)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
	}

	return false;
}

void Operation::Div::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Div] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", m_RhsSym->GetOffsetReg(), "%ecx");	AddCommentToPrevInstruction(o, "[Div] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Div] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");										AddCommentToPrevInstruction(o, "[Div] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Div] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", m_RhsSym->GetOffsetReg());					AddCommentToPrevInstruction(o, "[Div] divide and store result in to EDX:EAX");
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

bool Operation::Mod::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr % *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x % 0)
		if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
	}

	return false;
}

void Operation::Mod::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Mod] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", m_RhsSym->GetOffsetReg(), "%ecx");	AddCommentToPrevInstruction(o, "[Mod] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Mod] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");										AddCommentToPrevInstruction(o, "[Mod] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[Mod] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", m_RhsSym->GetOffsetReg());					AddCommentToPrevInstruction(o, "[Mod] divide and store result in to EDX:EAX");
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

#pragma endregion Multiplicative
#pragma region AdditiveEqual

bool Operation::PlusEqual::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr + *m_RhsSym->constPtr;
		m_LhsSym->constPtr = new int(res);
		return true;
	}
	// Check obsolete expression?
	else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
	{
		return true;
	}
	// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
	else if (m_LhsSym->constPtr)
	{
		delete m_LhsSym->constPtr;
		m_LhsSym->constPtr = nullptr;
	}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[PlusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[PlusEqual] save result into " + m_LhsSym->varName);
}

bool Operation::MinusEqual::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr - *m_RhsSym->constPtr;
		m_LhsSym->constPtr = new int(res);
		return true;
	}
	// Check obsolete expression?
	else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
	{
		return true;
	}
	// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
	else if (m_LhsSym->constPtr)
	{
		delete m_LhsSym->constPtr;
		m_LhsSym->constPtr = nullptr;
	}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MinusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MinusEqual] save result into " + m_LhsSym->varName);
}
#pragma endregion AdditiveEqual
#pragma region MultiplicativeEqual
bool Operation::MulEqual::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr * *m_RhsSym->constPtr;
		m_LhsSym->constPtr = new int(res);
		return true;
	}
	else
	{
		// Check obsolete expression?
		if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
		{
			return true;
		}
		else if (m_RhsSym->constPtr)
		{
			const int value = *m_RhsSym->constPtr;
			if (value == 1)
				return true;
			else if (value == 0)
			{
				m_LhsSym->constPtr = new int(0); //TODO: check if this is ok and make sure the lhs doesnt stay a const when its edited later
				return true;
			}
		}
		// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
		else if (m_LhsSym->constPtr)
		{
			delete m_LhsSym->constPtr;
			m_LhsSym->constPtr = nullptr;
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[MulEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[MulEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MulEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MulEqual] save result into " + m_LhsSym->varName);
}

bool Operation::DivEqual::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr / *m_RhsSym->constPtr;
		m_LhsSym->constPtr = new int(res);
		return true;
	}
	else
	{
		// Check obsolete expression?
		if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
			return true;
		else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 1)
			return true;
		// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
		else if (m_LhsSym->constPtr)
		{
			delete m_LhsSym->constPtr;
			m_LhsSym->constPtr = nullptr;
		}
	}

	return false;
}

void Operation::DivEqual::GenerateASM(std::ostream& o)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");				AddCommentToPrevInstruction(o, "[DivEqual] move " + m_LhsSym->varName + " into EAX");

	// How "cltd" works and why im not using EDX for rhsSym
	// https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
	if (m_RhsSym->varType == "char")
	{
		o << FormatInstruction("movsbl", m_RhsSym->GetOffsetReg(), "%ecx");		AddCommentToPrevInstruction(o, "[DivEqual] move " + m_RhsSym->varName + " into ECX");
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[DivEqual] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", "%ecx");										AddCommentToPrevInstruction(o, "[DivEqual] divide and store result in to EDX:EAX");
	}
	else
	{
		o << FormatInstruction("cltd");													AddCommentToPrevInstruction(o, "[DivEqual] sign-extends EAX into EDX:EAX");
		o << FormatInstruction("idivl", m_RhsSym->GetOffsetReg());						AddCommentToPrevInstruction(o, "[DivEqual] divide and store result in to EDX:EAX");
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
#pragma endregion BasicOperations

#pragma region BitwiseOperations
bool Operation::BitwiseAnd::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr & *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	// Check obsolete expression?
	else if ((m_LhsSym->constPtr && *m_LhsSym->constPtr == 0) || (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0))
	{
		m_ResultSym->constPtr = new int(0);
		return true;
	}
	return false;
}

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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("andl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseAnd] bitwise AND op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseAnd] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseOr::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr | *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	// Check obsolete expression?
	else
	{
		if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
		else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("orl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseOr] bitwise OR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseOr] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseXor::PropagateConst()
{
	if (m_LhsSym->constPtr && m_RhsSym->constPtr)
	{
		const int res = *m_LhsSym->constPtr ^ *m_RhsSym->constPtr;
		m_ResultSym->constPtr = new int(res);
		return true;
	}
	// Check obsolete expression?
	else
	{
		if (m_LhsSym->constPtr && *m_LhsSym->constPtr == 0)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
		else if (m_RhsSym->constPtr && *m_RhsSym->constPtr == 0)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
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

	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, m_RhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("xorl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseXor] bitwise XOR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseXor] save result into " + m_ResultSym->varName);
}
#pragma endregion BitwiseOperations

//TODO: implement assembly of branch related stuff when adding branch related stuff
#pragma region cmpOperations
bool Operation::LessThan::PropagateConst()
{
	return false;
}

void Operation::LessThan::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::GreaterThan::PropagateConst()
{
	return false;
}

void Operation::GreaterThan::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::Equal::PropagateConst()
{
	return false;
}

void Operation::Equal::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::NotEqual::PropagateConst()
{
	return false;
}

void Operation::NotEqual::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::LessOrEqual::PropagateConst()
{
	return false;
}

void Operation::LessOrEqual::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::GreaterOrEqual::PropagateConst()
{
	return false;
}

void Operation::GreaterOrEqual::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}

bool Operation::Not::PropagateConst()
{
	return false;
}

void Operation::Not::GenerateASM(std::ostream& o)
{
	throw NotImplementedException();
}
#pragma endregion cmpOperations

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

/// formats assembly instuction
/// @returns"	instruction		param1, param2"
std::string Instruction::FormatInstruction(std::string instr, std::string param1, std::string param2)
{
	return std::format("\t\t{}\t{}, {}\n", instr, param1, param2);
}

/// formats assembly instuction
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
