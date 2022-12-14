#include "BasicBlock.h"
#include "Instruction.h"

BasicBlock::BasicBlock(ControlFlowGraph* cfg, std::string label)
	: m_CFG(cfg)
	, m_Label(std::move(label))
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

void BasicBlock::AddInstr(Instruction::Operation op, std::string dest, std::vector<std::string> params, Scope* sT)
{
	m_Instructions.emplace_back(new Instruction(this, op, dest, std::move(params), sT));
}

void BasicBlock::AddInstr(Instruction* instr)
{
	m_Instructions.emplace_back(instr);
}
