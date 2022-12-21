#include "Instruction.h"
#include <cassert>
#include <utility>
#include "BasicBlock.h"
#include "../SymbolTable.h"


//https://www.cs.virginia.edu/~evans/cs216/guides/x86.html

Instruction::Instruction(BasicBlock* bb, Operation op, std::string dest, std::vector<std::string> params, Scope* scope)
	: m_BasicBlock(bb)
	, m_Scope(scope)
	, m_Op(op)
	, m_Dest(std::move(dest))
	, m_Params(std::move(params))
{
}

void Instruction::GenerateASM(std::ostream& o)
{
	switch (m_Op)
	{
	case Operation::Prologue:
		Prologue(o);
		break;
	case Operation::WriteConst:
		WriteCost(o);
		break;
	case Operation::ReadParam:
		ReadParam(o);
		break;
	case Operation::WriteParam:
		WriteParam(o);
		break;
	case Operation::Call:
		Call(o);
		break;
	case Operation::Return:
		Return(o);
		break;
	case Operation::LessThan:
		throw UnImplementedInstr_Exception("LessThan");
		break;
	case Operation::GreaterThan:
		throw UnImplementedInstr_Exception("GreaterThan");
		break;
	case Operation::BitwiseAnd:
		throw UnImplementedInstr_Exception("BitwiseAnd");
		break;
	case Operation::BitwiseOr:
		throw UnImplementedInstr_Exception("BitwiseOr");
		break;
	case Operation::BitwiseXor:
		throw UnImplementedInstr_Exception("BitwiseXor");
		break;
	case Operation::Equal:
		throw UnImplementedInstr_Exception("Equal");
		break;
	case Operation::NotEqual:
		throw UnImplementedInstr_Exception("NotEqual");
		break;
	case Operation::LessOrEqual:
		throw UnImplementedInstr_Exception("LessOrEqual");
		break;
	case Operation::GreaterOrEqual:
		throw UnImplementedInstr_Exception("GreaterOrEqual");
		break;
	case Operation::Assign:
		Assign(o);
		break;
	case Operation::Plus:
		Plus(o);
		break;
	case Operation::Minus:
		throw UnImplementedInstr_Exception("Minus");
		break;
	case Operation::Mul:
		throw UnImplementedInstr_Exception("Mul");
		break;
	case Operation::Div:
		throw UnImplementedInstr_Exception("Div");
		break;
	case Operation::Mod:
		throw UnImplementedInstr_Exception("Mod");
		break;
	case Operation::PlusEqual:
		throw UnImplementedInstr_Exception("PlusEqual");
		break;
	case Operation::MinusEqual:
		throw UnImplementedInstr_Exception("MinusEqual");
		break;
	case Operation::MulEqual:
		throw UnImplementedInstr_Exception("MulEqual");
		break;
	case Operation::DivEqual:
		throw UnImplementedInstr_Exception("DivEqual");
		break;
	case Operation::ExclamationMark:
		throw UnImplementedInstr_Exception("ExclamationMark");
		break;
	case Operation::Negate:
		throw UnImplementedInstr_Exception("Negate");
		break;
	case Operation::Declaration:
		throw UnImplementedInstr_Exception("Declaration");
		break;
	default:
		assert(!"should never get here!"); //!"string" evaluates to false
		return;
	}
}

// This prologue sets up the stack frame for the function, which will be used to access local
// variables and pass arguments to other functions. It also saves the old base pointer, which
// will be used to restore the previous stack frame when the function returns.
void Instruction::Prologue(std::ostream& o)
{
	const std::string label = m_Params.at(0);

	// .globl my_function, declares my_function as a global symbol, meaning it can be called from other files.
	o << ".globl " << label << '\n';
	// '.type' directive is used to specify the type of a symbol '@function' Indicates that the symbol is a function.
	o << ".type	" << label << ", @function" << '\n';
	// 'my_function:' labels the start of the function.
	o << label << ":" << '\n';

	o << FormatInstruction("pushq", "%rbp"); AddCommentToPrevInstruction(o, "Save the old base pointer");
	o << FormatInstruction("movq", "%rsp", "%rbp"); AddCommentToPrevInstruction(o, "Set the base pointer to the current stack pointer");

	// Keep the size a multiple of 16 for mem alignment reasons
	const int size = RoundUpToMultipleOf16(m_Scope->GetScopeSize());
	if (m_BasicBlock->GetFunction()->hasFuncCall && size > 0)
	{
		o << FormatInstruction("subq", "$" + std::to_string(size), "%rsp"); AddCommentToPrevInstruction(o, "Make room on the stack for local variables");
	}
}

//https://scottc130.medium.com/implementing-functions-in-x86-assembly-a2fb7315e2e0
void Instruction::Return(std::ostream& o)
{
	//check if we're actually returning anything
	if (!m_Params.empty())
	{
		// Move resultName in to EAX (return register)

		const std::string param = m_Params[0];

		// Returning a variable
		if (m_Scope->HasSymbol(param))
		{
			const Symbol* source = m_Scope->GetSymbol(param);
			const std::string sourceLoc = source->constPtr ? "$" + std::to_string((*source->constPtr)) : source->GetOffsetReg();
	
			o << FormatInstruction("movl", sourceLoc, "%eax");
		}
		// Returning a const value
		else
		{
			// Convert const to right value
			const bool isChar = m_BasicBlock->GetFunction()->returnType == "char";
			const int constValue = isChar ? static_cast<int>(param[1]) : stoi(param);

			o << FormatInstruction("movl", "$" + std::to_string(constValue), "%eax");
		}
	}

	// Move stack pointer back to where it was before the function
	o << FormatInstruction("movq", "%rbp", "%rsp");
	//https://stackoverflow.com/questions/4584089/what-is-the-function-of-the-push-pop-instructions-used-on-registers-in-x86-ass
	// Move base pointer back to where it was before the function
	o << FormatInstruction("popq", "%rbp");
	o << FormatInstruction("ret");
}

void Instruction::Call(std::ostream& o)
{
	const std::string label = m_Params.at(0);
	const Symbol* tempSym = m_Scope->GetSymbol(m_Dest);
	const int numParams = std::stoi(m_Params.at(1));

	// Write ASM instructions
	o << FormatInstruction("call", label); AddCommentToPrevInstruction(o, "[Call]");
	if (numParams > 6) //TODO: test this properly!
	{
		// *8 because we use 'addq' in writeParam so we push 64bits at a time (and we do addq bc we add to RSP which is 64bit)
		o << FormatInstruction("addq", '$' + std::to_string((numParams - m_AmountOfRegisters) * 8), "%rsp"); AddCommentToPrevInstruction(o, "[Call] Move SP back to position where it was before pushing the params that didn't fit in registers");
	}

	//NOTE: only needed when function actually returns something
	//save function return value
	if (m_BasicBlock->GetFunction()->returnType != "void") //TODO: maybe unessesery as it might be optimized away anyway
	{
		o << FormatInstruction("movl", "%eax", tempSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[Call] Save Func Return value into " + tempSym->varName);
	}

}

void Instruction::WriteParam(std::ostream& o)
{
	int currParamIdx = std::stoi(m_Params.at(1));
	Symbol* givenParamSym = m_Scope->GetSymbol(m_Params.at(0)); //load the value/symbol that's given as parameter to the function

	// Use registers as long as there are available
	if (currParamIdx < m_AmountOfRegisters)
	{
		const std::string reg = m_TypeRegisterMap.at(givenParamSym->varType)[currParamIdx];
		const std::string movInstr = (givenParamSym->varType == "char") ? "movb" : "movl";

		std::string param;
		if (givenParamSym->constPtr)
			param = '$' + std::to_string(*givenParamSym->constPtr);
		else
			param = givenParamSym->GetOffsetReg();

		//TODO: Test this for char's
		// Move parameters in to registers
		o << FormatInstruction(movInstr, param, reg); AddCommentToPrevInstruction(o, "[WriteParam] move param:" + givenParamSym->varName + " into " + reg);
	}
	// If we ran out of registers push parameters on the stack
	else 
	{
		//NOTE: THESE PUSH'S DON'T HAVE ANY POP'S
		//2nd NOTE: (they don't need any as they will be overwritten by next function as the sp moves behind them (See: Instruction::Call))
		if (givenParamSym->constPtr)
		{
			o << FormatInstruction("pushq", '$' + std::to_string(*givenParamSym->constPtr));
		}
		else
		{
			if (givenParamSym->varType == "char") 
			{
				o << FormatInstruction("movzbl", givenParamSym->GetOffsetReg(), "%eax");
				o << FormatInstruction("pushq", "%rax");
			}
			else 
			{
				o << FormatInstruction("pushq", givenParamSym->GetOffsetReg());
			}

		}
		AddCommentToPrevInstruction(o, "[WriteParam] Push parameter on stack as theres no free register left");
	}
}

void Instruction::ReadParam(std::ostream& o)
{
	int currParamIdx = std::stoi(m_Params.at(1));
	Symbol* localParamSym = m_Scope->GetSymbol(m_Params.at(0));  //load the local symbol that's used as parameter of the function

	// Use registers as long as there are available
	if (currParamIdx < m_AmountOfRegisters) {

		const std::string reg = m_TypeRegisterMap.at(localParamSym->varType)[currParamIdx];
		const std::string movInstr = (localParamSym->varType == "char") ? "movb" : "movl";

		o << FormatInstruction(movInstr, reg, localParamSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[ReadParam] move " + reg + " into param:" + localParamSym->varName);
	}
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

void Instruction::WriteCost(std::ostream& o)
{
	const std::string constValueString = m_Params.at(0);
	const std::string tempSymName = m_Dest;

	const Symbol* tempSym = m_Scope->GetSymbol(tempSymName);
	const bool isChar = tempSym->varType == "char";

	o << FormatInstruction(isChar ? "movb" : "movl", "$" + constValueString, tempSym->GetOffsetReg()); AddCommentToPrevInstruction(o, "[WriteConst] move " + constValueString + " into " + tempSymName);
}

void Instruction::Assign(std::ostream& o)
{
	const Symbol* dest = m_Scope->GetSymbol(m_Dest);
	const Symbol* source = m_Scope->GetSymbol(m_Params.at(0));

	const std::pair<std::string, std::string> instPair = GetMoveInstr(dest->varType, source->varType);
	const std::string reg = dest->varType == "int" ? "%eax" : "%al";

	o << FormatInstruction(instPair.first, source->GetOffsetReg(), reg);
	o << FormatInstruction(instPair.second, reg, dest->GetOffsetReg());
}

void Instruction::Plus(std::ostream& o)
{
	const Symbol* lhsSym = m_Scope->GetSymbol(m_Params.at(0));
	const Symbol* rhsSym = m_Scope->GetSymbol(m_Params.at(1));
	const Symbol* resultSym = m_Scope->GetSymbol(m_Dest);

	const std::string movInstr1 = GetMoveInstr(lhsSym->varType);
	const std::string movInstr2 = GetMoveInstr(rhsSym->varType);

	o << FormatInstruction(movInstr1, lhsSym->GetOffsetReg(), "%eax");			AddCommentToPrevInstruction(o, "[Plus] move " + lhsSym->varName + " into EAX");
	o << FormatInstruction(movInstr2, rhsSym->GetOffsetReg(), "%edx");			AddCommentToPrevInstruction(o, "[Plus] move " + rhsSym->varName + " into EDX");
	o << FormatInstruction("addl", "%edx", "%eax");						AddCommentToPrevInstruction(o, "[Plus] add values together");
	o << FormatInstruction("movl", "%eax", resultSym->GetOffsetReg());	AddCommentToPrevInstruction(o, "[Plus] save result into " + resultSym->varName);
}


///---------------------------
///	Utils
///---------------------------
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
	o << "\t#" + comment + '\n'; //technically overwrite \n
}

int Instruction::RoundUpToMultipleOf16(const int x)
{
	return (x + 15) & ~15;
}