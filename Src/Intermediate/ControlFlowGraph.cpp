#include "ControlFlowGraph.h"
#include "BasicBlock.h"

ControlFlowGraph::ControlFlowGraph(bool optimized)
	: m_Optimized(optimized)
{
	CreateNewCurrBB();
}

ControlFlowGraph::~ControlFlowGraph()
{
	for (auto* basicBlock : m_BasicBlocks)
	{
		delete basicBlock;
		basicBlock = nullptr;
	}
}

BasicBlock* ControlFlowGraph::CreateNewCurrBB()
{
	auto* bb = new BasicBlock(this, ".BasicBlock" + std::to_string(m_BasicBlocks.size()));
	m_BasicBlocks.emplace_back(bb);
	m_CurrentBB = bb;
	return bb;
}
