#include "BasicBlock.h"
#include "ControlFlowGraph.h"
#include "Instruction.h"
#include "../Scope.h"

BasicBlock::BasicBlock(ControlFlowGraph* cfg, std::string label, Function* fn)
	: m_CFG(cfg)
	, m_Label(std::move(label))
	, m_Function(fn)
{
}

BasicBlock::~BasicBlock()
{
	for (auto block : m_Entries)
	{
		if (block->m_ExitTrue == this)
		{
			block->m_ExitTrue = nullptr;
		}
		if (block->m_ExitFalse == this)
		{
			block->m_ExitFalse = nullptr;
		}
	}
	if (m_ExitTrue)
	{
		std::erase(m_ExitTrue->m_Entries, this);
	}
	if (m_ExitFalse)
	{
		std::erase(m_ExitFalse->m_Entries, this);
	}

	for (auto* instruction : m_Instructions)
	{
		delete instruction;
		instruction = nullptr;
	}
}

//void BasicBlock::AddInstr(Instruction::Operation op, std::string dest, std::vector<std::string> params, Scope* sT)
//{
//	m_Instructions.emplace_back(new Instruction(this, op, dest, std::move(params), sT));
//}

void BasicBlock::AddInstr(Instruction* instr)
{
	m_Instructions.emplace_back(instr)->SetBasicBlock(this);
}

void BasicBlock::OptimizeIR()
{
	if constexpr (g_OptimizeConstPropagation)
	{
		//compute in set
		ComputeInSet(m_Entries);

		for (auto i = m_Instructions.begin(); i != m_Instructions.end();)
		{
			if ((*i)->PropagateConst())
				i = m_Instructions.erase(i);
			else
				++i;
		}

		m_ConstUninitilaized = false;
	}
}

void BasicBlock::GenerateX86(std::ostream& o)
{
	if (m_Label[0] == '.')
	{
		o << m_Label << ":\n";
	}
	for (auto* instr : m_Instructions)
		instr->GenerateASM(o);
}
