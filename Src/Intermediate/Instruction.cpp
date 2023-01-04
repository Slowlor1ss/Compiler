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

bool Operation::Return::PropagateConst()
{
	if(!m_ReturnParam.empty())
	{
		const auto* sym = m_Scope->GetSymbol(m_ReturnParam);
		if (sym && m_BasicBlock->GetConst(sym).has_value())
		{
			m_ReturnParam = std::to_string(sym->constVal.value());
		}
	}

	return false;
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
			o << FormatInstruction("movl", source->GetOffsetReg(), "%eax");
		}
		// Returning a const value
		else
		{
			o << FormatInstruction("movl", "$" + m_ReturnParam, "%eax");
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

bool Operation::WriteParam::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_Sym).has_value())
	{
		m_ConstVal = m_Sym->constVal;
	}
	return false;
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
		char a = '%d/0';
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

bool Operation::WriteConst::PropagateConst()
{
	//TODO: remove code in visitor add code here dont forget to delete temp var
	return false;
}

void Operation::WriteConst::GenerateASM(std::ostream& o)
{
	if constexpr (g_RemoveDeadcode)
	{
		if (!m_IsUsed)
			return;
	}

	const bool isChar = m_Sym->varType == "char";

	o << FormatInstruction(isChar ? "movb" : "movl", "$" + m_ValueString, m_Sym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[WriteConst] move " + m_ValueString + " into " + m_Sym->varName);

}

#pragma region BasicOperations
bool Operation::Assign::PropagateConst()
{
	//note to self while making this:
	// - only has to be done once per bb per variable
	// - what if const propagation hasn't been done yet for the entry block like in a while loop (think about how while loops will be implemented)

	// If we find it in out const table that we already checked it
	//if (m_BasicBlock->FindConst(m_SourceSym->varName))
	//	return;
	//
	//if (m_SourceSym->constVal)
	//{
	//	bool m_ConstMismatch = false;
	//	for (auto* entryBlock : m_BasicBlock->GetEntryBlocks())
	//	{
	//		if(auto prevConst = entryBlock->FindConst(m_SourceSym->varName))
	//		{
	//			if (prevConst.has_value())
	//			{
	//				if (prevConst != m_SourceSym->constVal.value())
	//				{
	//					//const mismatch
	//					m_ConstMismatch = true;
	//					break;
	//				}
	//			}
	//			// If its const here but not in the prev block the value is by default a mismatch
	//			else
	//			{
	//				m_ConstMismatch = true;
	//				break;
	//			}
	//		}
	//	}
	//	if(m_ConstMismatch)
	//	{
	//		// For every block that flows in to this one add an instruction that stores the right value of our previously const value so that the right value will be in memory
	//		for (auto* entryBlock : m_BasicBlock->GetEntryBlocks())
	//		{
	//			if (auto prevConst = entryBlock->FindConst(m_SourceSym->varName)) //if its found
	//			{
	//				if (prevConst.has_value()) // and has a const value -> write that value
	//				{
	//					m_BasicBlock->AddInstr(new WriteConst(m_SourceSym, std::to_string(*prevConst), m_Scope)); //TODO: fix; technically scope isn't fully correct anymore but correct enough as the variable will exist in the given scope
	//				}
	//				//else don't do anything as it's value will already be written in that block
	//			}
	//			else
	//			{
	//				//if its not found we have to still write it at the end of that block so it is set for the future
	//				//TODO: maybe set it in the entry block of this entry so it would eg. be above the if?
	//				m_BasicBlock->AddInstr(new WriteConst(m_SourceSym, std::to_string(*m_SourceSym->constVal), m_Scope)); //TODO: fix; same
	//			}
	//		}
	//
	//		//no longer has a const value
	//		m_SourceSym->constVal = std::nullopt;
	//	}
	//}
	//
	//{
	//	bool constIsUnknown{ false };
	//	for (auto* entryBlock : m_BasicBlock->GetEntryBlocks())
	//	{
	//		if (!entryBlock->GetAsmInitialized())
	//		{
	//			constIsUnknown = true;
	//		}
	//	}
	//
	//	bool SafeToUse(Symbol* Symbol, int newValue)
	//	{
	//		if (constIsUnknown && m_DestSym->constVal)
	//		{
	//			m_DestSym->constVal = std::nullopt;
	//			m_BasicBlock->AddAndUpdateConst(m_DestSym->varName, std::nullopt);
	//			return false;
	//		}
	//
	//		//auto [hasConst, constValue] = m_BasicBlock->FindConst(m_DestSym->varName);
	//		//if (hasConst && constValue.value() == m_DestSym->constVal.value())
	//		//{
	//		//	return true;
	//		//}
	//
	//		if(m_DestSym->constVal != m_SourceSym->constVal)
	//		{
	//			
	//		}
	//
	//		//check if all enties agree on the value
	//		bool m_ConstMismatch = false;
	//		for (auto* entryBlock : m_BasicBlock->GetEntryBlocks())
	//		{
	//			if (auto prevConst = entryBlock->FindConst(m_DestSym->varName))
	//			{
	//				if (prevConst.has_value())
	//				{
	//					if (prevConst != m_DestSym->constVal.value())
	//					{
	//						//const mismatch
	//						m_ConstMismatch = true;
	//						break;
	//					}
	//				}
	//				// If its const here but not in the prev block the value is by default a mismatch
	//				else
	//				{
	//					m_ConstMismatch = true;
	//					break;
	//				}
	//			}
	//		}
	//		if (m_ConstMismatch)
	//		{
	//			// For every block that flows in to this one add an instruction that stores the right value of our previously const value so that the right value will be in memory
	//			for (auto* entryBlock : m_BasicBlock->GetEntryBlocks())
	//			{
	//				auto [entrieHasConst, entryConstValue] = entryBlock->FindConst(m_DestSym->varName);
	//				if (entrieHasConst) //if its found
	//				{
	//					if (entryConstValue.has_value()) // and has a const value -> write that value
	//					{
	//						m_BasicBlock->AddInstr(new WriteConst(m_DestSym, std::to_string(*entryConstValue), m_Scope)); //TODO: fix; technically scope isn't fully correct anymore but correct enough as the variable will exist in the given scope
	//					}
	//					//else don't do anything as it's value will already be written in that block
	//				}
	//				else
	//				{
	//					//if its not found we have to still write it at the end of that block so it is set for the future
	//					//TODO: maybe set it in the entry block of this entry so it would eg. be above the if?
	//					m_BasicBlock->AddInstr(new WriteConst(m_DestSym, std::to_string(*m_DestSym->constVal), m_Scope)); //TODO: fix; same
	//				}
	//			}
	//
	//			m_BasicBlock->AddAndUpdateConst(m_DestSym->varName, m_SourceSym->constVal);
	//			//no longer has a const value
	//			m_DestSym->constVal = std::nullopt;
	//		}
	//
	//
	//	}
	//}

	//TODO: remove
	//const auto constValue = m_BasicBlock->FindConst(m_SourceSym->varName);
	//if (constValue.has_value())
	//{
	//	m_DestSym->constVal = constValue;
	//	m_BasicBlock->AddAndUpdateConst(m_DestSym, constValue);
	//	//TODO: do an is used check and if it is used then write const
	//	//TODO:	else remove
	//	//if (m_DestSym->isUsed)
	//	//{
	//		m_BasicBlock->ReplaceInstruction(this, new Operation::WriteConst(m_DestSym, std::to_string(*constValue), m_Scope));
	//	//}
	//}
	//else
	//TODO: cheack on this
	//if(m_SourceSym->GetConst().has_value())
	if(m_BasicBlock->GetConst(m_SourceSym).has_value())
	{
		//if (m_DestSym->isUsed)//TODO: uncomment
		//{
			//m_DestSym->constVal = m_SourceSym->constVal;
			auto* constInstr = new Operation::WriteConst(m_DestSym, std::to_string(*m_SourceSym->constVal), m_Scope);
			m_BasicBlock->AddAndUpdateConst(m_DestSym, m_SourceSym->constVal, constInstr);
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//}
		//else
		//	return true;
	}
	else
	{
		//m_DestSym->constVal = std::nullopt;
		m_BasicBlock->AddAndUpdateConst(m_DestSym, std::nullopt, nullptr);
	}

	//TODO: clean this up and make it work
	// If it is a scope variable assign with a const
	//if (m_SourceSym->constVal)
	//{
	//	if constexpr (g_ConstPropagationAssignment)
	//	{
	//		//TODO: we dont delete dest constptr if it already had one
	//		m_DestSym->constVal = m_SourceSym->constVal.value();
	//		return true;
	//	}
	//	else
	//	{
	//		m_BasicBlock->ReplaceInstruction(this, new Operation::WriteConst(m_DestSym, std::to_string(*m_SourceSym->constVal), m_Scope)); 
	//	}
	//}
	//// If the destination still has a constPtr but we assign a non const to it then destination should also no longer be const
	//else if(m_DestSym->constVal)
	//{
	//	m_DestSym->constVal = std::nullopt;
	//}
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
	m_BasicBlock->AddAndUpdateConst(m_Sym, std::nullopt, nullptr);
	return true;
}

//TODO: remove
//void PropCostSym(Symbol* toSym, Symbol* FromSym, BasicBlock* bb)
//{
//	const auto constValue = bb->FindConst(FromSym->varName);
//	if (constValue.has_value())
//	{
//		const int res = -constValue.value();
//		toSym->constVal = res;
//	}
//	else if (FromSym->constVal.has_value())
//	{
//		const int res = -FromSym->constVal.value();
//		toSym->constVal = res;
//		bb->AddAndUpdateConst(toSym, toSym->constVal);
//	}
//}

bool Operation::Negate::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_OrigSym).has_value())
	{
		const int res = -m_OrigSym->constVal.value();
		m_TempSym->constVal = res;
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
bool Operation::Additive::CheckObsoleteExpr()
{
	// Check obsolete expression? (eg. x + 0 || x - 0 || 0 + x || etc...)
	if (m_BasicBlock->GetConst(m_LhsSym).has_value())
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;

		// Could just be written as if(m_LhsSym->constVal.value() == 0) but it maybe a bit more readable if we first explicitly check if it has a value
		if(m_LhsSym->constVal.value() == 0)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;

		if(m_RhsSym->constVal.value() == 0)
		{
			*m_ResultSym = *m_LhsSym;
			return true;
		}
	}
	return false;
}

bool Operation::Plus::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() + m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");							AddCommentToPrevInstruction(o, "[Plus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");							AddCommentToPrevInstruction(o, "[Plus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");					AddCommentToPrevInstruction(o, "[Plus] add values together");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Plus] save result into " + m_ResultSym->varName);
}

bool Operation::Minus::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() - m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[Minus] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[Minus] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[Minus] subtract values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Minus] save result into " + m_ResultSym->varName);
}
#pragma endregion Additive
#pragma region Multiplicative
//TODO: todolist change m_LhsSym->constVal to bb->getconst and add m_costval in case only one param is const
bool Operation::Mul::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() * m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x * 1 || 1 * x || 0 * x || ...)
		if (m_BasicBlock->GetConst(m_LhsSym).has_value())
		{
			m_ConstVal = m_LhsSym->constVal;
			m_LhsIsConst = true;
			if (m_ConstVal == 1)
			{
				*m_ResultSym = *m_RhsSym;
				return true;
			}
			else if(m_ConstVal == 0)
			{
				m_ResultSym->constVal = 0;
				return true;
			}
		}
		else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
		{
			m_ConstVal = m_RhsSym->constVal;
			m_LhsIsConst = false;
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");									AddCommentToPrevInstruction(o, "[Mul] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[Mul] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[Mul] multiply values");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[Mul] save result into " + m_ResultSym->varName);
}

bool Operation::Div::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() / m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x / 1 || 0 / x )
		if (m_BasicBlock->GetConst(m_LhsSym).has_value())
		{
			m_ConstVal = m_LhsSym->constVal;
			m_LhsIsConst = true;
			if (m_ConstVal == 0)
			{
				m_ResultSym->constVal = 0;
				return true;
			}
		}
		else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
		{
			m_ConstVal = m_RhsSym->constVal;
			m_LhsIsConst = false;

			if (m_RhsSym->constVal.value() == 1)
			{
				*m_ResultSym = *m_LhsSym;
				return true;
			}
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

bool Operation::Mod::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() % m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	else
	{
		// Check obsolete expression? (eg. x % 0)
		if (m_BasicBlock->GetConst(m_RhsSym).has_value())
		{
			m_ConstVal = m_RhsSym->constVal;
			m_LhsIsConst = false;

			if (m_RhsSym->constVal.value() == 0)
			{
				*m_ResultSym = *m_LhsSym;
				return true;
			}
		}
		else if (m_BasicBlock->GetConst(m_LhsSym).has_value())
		{
			m_ConstVal = m_LhsSym->constVal;
			m_LhsIsConst = true;
		}
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

#pragma endregion Multiplicative
#pragma region AdditiveEqual

bool Operation::PlusEqual::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() + m_RhsSym->constVal.value();
		//m_LhsSym->constVal = res;
		auto* constInstr = new Operation::WriteConst(m_LhsSym, std::to_string(res), m_Scope);
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, res, constInstr);
		//if constexpr (g_ConstPropagationAssignment)
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//else
		//	return true;
	}
	// Check obsolete expression?
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;

		if (m_RhsSym->constVal.value() == 0)
			return true;
	}
	// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
	else if (m_LhsSym->constVal)
	{
		//m_LhsSym->constVal = std::nullopt;
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, std::nullopt, nullptr);
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

	const std::string rhs = m_ConstVal.has_value() ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[PlusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[PlusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[PlusEqual] save result into " + m_LhsSym->varName);
}

bool Operation::MinusEqual::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() - m_RhsSym->constVal.value();
		//m_LhsSym->constVal = res;
		auto* constInstr = new Operation::WriteConst(m_LhsSym, std::to_string(res), m_Scope);
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, res, constInstr);
		//if constexpr (g_ConstPropagationAssignment)
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//else
		//	return true;
	}
	// Check obsolete expression?
	else  if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;

		if (m_RhsSym->constVal.value() == 0)
			return true;
	}
	// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
	else if (m_LhsSym->constVal)
	{
		//m_LhsSym->constVal = std::nullopt;
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, std::nullopt, nullptr);
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

	const std::string rhs = m_ConstVal.has_value() ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[MinusEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("subl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MinusEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MinusEqual] save result into " + m_LhsSym->varName);
}
#pragma endregion AdditiveEqual
#pragma region MultiplicativeEqual
bool Operation::MulEqual::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() * m_RhsSym->constVal.value();
		//m_LhsSym->constVal = res;
		auto* constInstr = new Operation::WriteConst(m_LhsSym, std::to_string(res), m_Scope);
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, res, constInstr);
		//if constexpr (g_ConstPropagationAssignment)
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//else
		//	return true;
	}
	else
	{
		// Check obsolete expression?
		if (m_LhsSym->constVal && m_LhsSym->constVal.value() == 0)
		{
			return true;
		}
		else if (m_RhsSym->constVal)
		{
			m_ConstVal = m_RhsSym->constVal.value();
			if (m_ConstVal == 1)
				return true;
			else if (m_ConstVal == 0)
			{
				//m_LhsSym->constVal = 0;
				auto* constInstr = new Operation::WriteConst(m_LhsSym, "0", m_Scope);
				m_BasicBlock->AddAndUpdateConst(m_LhsSym, 0, constInstr);
				//if constexpr (g_ConstPropagationAssignment)
					m_BasicBlock->ReplaceInstruction(this, constInstr);
				//else
				//	return true;
			}
		}
		// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
		else if (m_LhsSym->constVal)
		{
			//m_LhsSym->constVal = std::nullopt;
			m_BasicBlock->AddAndUpdateConst(m_LhsSym, std::nullopt, nullptr);
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

	const std::string rhs = m_ConstVal.has_value() ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, m_LhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[MulEqual] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");									AddCommentToPrevInstruction(o, "[MulEqual] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("imull", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[MulEqual] add values together");
	o << FormatInstruction(movInstr3, reg3, m_LhsSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[MulEqual] save result into " + m_LhsSym->varName);
}

bool Operation::DivEqual::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() / m_RhsSym->constVal.value();
		//m_LhsSym->constVal = res;
		auto* constInstr = new Operation::WriteConst(m_LhsSym, std::to_string(res), m_Scope);
		m_BasicBlock->AddAndUpdateConst(m_LhsSym, res, constInstr);
		//if constexpr (g_ConstPropagationAssignment)
			m_BasicBlock->ReplaceInstruction(this, constInstr);
		//else
		//	return true;
	}
	else
	{
		// Check obsolete expression?
		if (m_LhsSym->constVal && m_LhsSym->constVal.value() == 0)
			return true;
		else if (m_RhsSym->constVal)
		{
			m_ConstVal = m_RhsSym->constVal.value();
			if (m_ConstVal == 1)
				return true;
		}
		// If the destination(lhs) still has a constPtr but we assign a non const to it then destination should also no longer be const
		else if (m_LhsSym->constVal)
		{
			m_BasicBlock->AddAndUpdateConst(m_LhsSym, std::nullopt, nullptr);
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

	const std::string rhs = m_ConstVal.has_value() ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
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
#pragma endregion BasicOperations

#pragma region BitwiseOperations
bool Operation::BitwiseAnd::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() & m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	// Check obsolete expression?
	else if (m_BasicBlock->GetConst(m_LhsSym).has_value())
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;

		if (m_LhsSym->constVal.value() == 0)
		{
			m_ResultSym->constVal = 0;
			return true;
		}
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;

		if (m_RhsSym->constVal.value() == 0)
		{
			m_ResultSym->constVal = 0;
			return true;
		}
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseAnd] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("andl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseAnd] bitwise AND op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseAnd] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseOr::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() | m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	// Check obsolete expression?
	else if(m_BasicBlock->GetConst(m_LhsSym).has_value())
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;

		if (m_LhsSym->constVal.value() == 0)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;
		
		if (m_RhsSym->constVal.value() == 0)
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseOr] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("orl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseOr] bitwise OR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseOr] save result into " + m_ResultSym->varName);
}

bool Operation::BitwiseXor::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = m_LhsSym->constVal.value() ^ m_RhsSym->constVal.value();
		m_ResultSym->constVal = res;
		return true;
	}
	// Check obsolete expression?
	else if (m_BasicBlock->GetConst(m_LhsSym).has_value())
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;

		if (m_LhsSym->constVal.value() == 0)
		{
			*m_ResultSym = *m_RhsSym;
			return true;
		}
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;

		if (m_RhsSym->constVal.value() == 0)
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

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
	o << FormatInstruction(movInstr1, lhs, "%eax");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_LhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhs, "%edx");			AddCommentToPrevInstruction(o, "[BitwiseXor] move " + m_RhsSym->varName + " into EDX");
	o << FormatInstruction("xorl", "%edx", "%eax");							AddCommentToPrevInstruction(o, "[BitwiseXor] bitwise XOR op");
	o << FormatInstruction(movInstr3, reg3, m_ResultSym->GetOffsetReg());			AddCommentToPrevInstruction(o, "[BitwiseXor] save result into " + m_ResultSym->varName);
}

#pragma endregion BitwiseOperations

//TODO: implement assembly of branch related stuff when adding branch related stuff
#pragma region cmpOperations
void Operation::Comparison::GenerateCmpASM(std::ostream& o, std::string cmpOpInstr, const std::string& OpName)
{
	const std::string movInstr1 = m_LhsSym->varType == "char" ? "movsbl" : "movl";
	const std::string movInstr2 = m_RhsSym->varType == "char" ? "movsbl" : "movl";

	const std::string lhs = (m_ConstVal.has_value() && m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_LhsSym->GetOffsetReg();
	const std::string rhs = (m_ConstVal.has_value() && !m_LhsIsConst) ? '$' + std::to_string(m_ConstVal.value()) : m_RhsSym->GetOffsetReg();
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
bool Operation::Comparison::PropagateCmpConst(const std::function<bool(bool, bool)>& operation)
{
	if (m_BasicBlock->GetConst(m_LhsSym).has_value() && m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		const int res = operation(m_LhsSym->constVal.value(), m_ResultSym->constVal.value());
		m_ResultSym->constVal = res;
		return true;
	}

	if (m_BasicBlock->GetConst(m_LhsSym).has_value())
	{
		m_ConstVal = m_LhsSym->constVal;
		m_LhsIsConst = true;
	}
	else if (m_BasicBlock->GetConst(m_RhsSym).has_value())
	{
		m_ConstVal = m_RhsSym->constVal;
		m_LhsIsConst = false;
	}
	return false;
}

bool Operation::LessThan::PropagateConst()
{
	return PropagateCmpConst(std::less<>());
}

void Operation::LessThan::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setl", "LessThan");
}

bool Operation::GreaterThan::PropagateConst()
{
	return PropagateCmpConst(std::greater<>());
}

void Operation::GreaterThan::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setg", "GreaterThan");
}

bool Operation::Equal::PropagateConst()
{
	return PropagateCmpConst(std::equal_to<>());
}

void Operation::Equal::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "sete", "Equal");
}

bool Operation::NotEqual::PropagateConst()
{
	return PropagateCmpConst(std::not_equal_to<>());
}

void Operation::NotEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setne", "NotEqual");
}

bool Operation::LessOrEqual::PropagateConst()
{
	return PropagateCmpConst(std::less_equal<>());
}

void Operation::LessOrEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setle", "LessOrEqual");
}

bool Operation::GreaterOrEqual::PropagateConst()
{
	return PropagateCmpConst(std::greater_equal<>());
}

void Operation::GreaterOrEqual::GenerateASM(std::ostream& o)
{
	GenerateCmpASM(o, "setge", "GreaterOrEqual");
}

bool Operation::Not::PropagateConst()
{
	if (m_BasicBlock->GetConst(m_OrigSym).has_value())
	{
		const int res = !m_OrigSym->constVal.value();
		m_TempSym->constVal = res;
		//TODO: maybe also delete origSym (will always be temp and set in write const) If we want to delete it add a delete sym to our scope
		return true;
	}
	return false;
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

#pragma endregion cmpOperations

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
