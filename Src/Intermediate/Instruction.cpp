#include "Instruction.h"
#include <cassert>
#include <utility>
#include "BasicBlock.h"


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
		DefaultNotImplementedOperation();
		break;
	case Operation::ReadParam:
		DefaultNotImplementedOperation();
		break;
	case Operation::WriteParam:
		DefaultNotImplementedOperation();
		break;
	case Operation::Call:
		DefaultNotImplementedOperation();
		break;
	case Operation::Return:
		Return(o);
		break;
	case Operation::LessThan:
		DefaultNotImplementedOperation();
		break;
	case Operation::GreaterThan:
		DefaultNotImplementedOperation();
		break;
	case Operation::BitwiseAnd:
		DefaultNotImplementedOperation();
		break;
	case Operation::BitwiseOr:
		DefaultNotImplementedOperation();
		break;
	case Operation::BitwiseXor:
		DefaultNotImplementedOperation();
		break;
	case Operation::Equal:
		DefaultNotImplementedOperation();
		break;
	case Operation::NotEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::LessOrEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::GreaterOrEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::Assign:
		Assign(o);
		break;
	case Operation::Plus:
		DefaultNotImplementedOperation();
		break;
	case Operation::Minus:
		DefaultNotImplementedOperation();
		break;
	case Operation::Mul:
		DefaultNotImplementedOperation();
		break;
	case Operation::Div:
		DefaultNotImplementedOperation();
		break;
	case Operation::Mod:
		DefaultNotImplementedOperation();
		break;
	case Operation::PlusEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::MinusEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::MulEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::DivEqual:
		DefaultNotImplementedOperation();
		break;
	case Operation::ExclamationMark:
		DefaultNotImplementedOperation();
		break;
	case Operation::Negate:
		DefaultNotImplementedOperation();
		break;
	case Operation::Declaration:
		DefaultNotImplementedOperation();
		break;
	default:
		assert(!"should never get here!"); //!"string" evaluates to false
		return;
	}
}

//https://scottc130.medium.com/implementing-functions-in-x86-assembly-a2fb7315e2e0
void Instruction::Return(std::ostream& o)
{
	//check if we're actually returning anything
	if (!m_Params.empty())
	{
		const std::string param = m_Params[0];

		// If we're returning a var
		if (m_Scope->HasSymbol(param))
		{
			Symbol* s1 = m_Scope->GetSymbol(param);
			if (s1->constPtr)
			{
				o << "	movl	$" << *s1->constPtr << ", %eax";
			}
			else
			{
				o << "	movl	" << s1->memoryOffset << "(%rbp), %eax";
			}
		}
		// If we're returning a const
		else
		{
			int constValue;
			const bool isChar = m_BasicBlock->GetFunction()->returnType == "char";
			if (isChar)
			{
				//only need to keep the value of the letter
				constValue = static_cast<int>(param[1]);
			}
			else
			{
				//get value of the number
				constValue = stoi(param.substr(1, param.size() - 1));
			}

			o << "	movl	$" << constValue << ", %eax";
		}
	}

	o << "	movq	%rbp, %rsp\n"; //TODO: remove my comment as i think its wrong NOTE: only need this line when actually returning a value in void functions you dont need this not the above
	//https://stackoverflow.com/questions/4584089/what-is-the-function-of-the-push-pop-instructions-used-on-registers-in-x86-ass
	o << "	popq	%rbp\n";
	o << "	ret\n";
}

void Instruction::Assign(std::ostream& o)
{
	Symbol* dest = m_Scope->GetSymbol(m_Dest);
	Symbol* source = m_Scope->GetSymbol(m_Params.at(0));

	o << " movl " << source->memoryOffset << "(%rbp), %eax";
	o << " movl %eax, " << dest->memoryOffset << "(%rbp)";

}
