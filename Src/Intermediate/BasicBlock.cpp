#include "BasicBlock.h"
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
	for (auto i = m_Instructions.begin(); i != m_Instructions.end();)
	{
		if ((*i)->PropagateConst())
			i = m_Instructions.erase(i);
		else
			++i;
	}
}

void BasicBlock::GenerateX86(std::ostream& o) const
{
	for (auto* instr : m_Instructions)
		instr->GenerateASM(o);
}
