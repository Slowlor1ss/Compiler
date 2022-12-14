#include "ControlFlowGraph.h"
#include "BasicBlock.h"

ControlFlowGraph::ControlFlowGraph(bool optimized)
	: m_Optimized(optimized)
{
	CreateNewCurrBB("Global", nullptr);
}

ControlFlowGraph::~ControlFlowGraph()
{
	for (auto* basicBlock : m_BasicBlocks)
	{
		delete basicBlock;
		basicBlock = nullptr;
	}
}

BasicBlock* ControlFlowGraph::CreateNewCurrBB(std::string name, Function* fn)
{
	auto* bb = new BasicBlock(this, ".BasicBlock_" + name, fn);
	m_BasicBlocks.emplace_back(bb);
	m_CurrentBB = bb;
	return bb;
}

BasicBlock* ControlFlowGraph::CreateNewCurrBB(Function* fn)
{
	auto* bb = new BasicBlock(this, ".BasicBlock_" + std::to_string(m_BasicBlocks.size()), fn);
	m_BasicBlocks.emplace_back(bb);
	m_CurrentBB = bb;
	return bb;
}
