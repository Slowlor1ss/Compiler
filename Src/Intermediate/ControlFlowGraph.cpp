#include "ControlFlowGraph.h"
#include "BasicBlock.h"

ControlFlowGraph::ControlFlowGraph(bool optimized)
	: m_Optimized(optimized)
{
	//CreateNewCurrBB("Global", nullptr);
}

ControlFlowGraph::~ControlFlowGraph()
{
	for (auto* basicBlock : m_BasicBlocks)
	{
		delete basicBlock;
		basicBlock = nullptr;
	}
}

BasicBlock* ControlFlowGraph::CreateNewCurrBB(Function* fn)
{
	//std::string labelName{fn->funcName};
	////Generate unique name for every function buy adding the first letter of the param types
	//if (!fn->parameterTypes.empty())
	//	labelName += +"_";
	//for (auto& pt : fn->parameterTypes)
	//	labelName += pt[0];

	auto* bb = new BasicBlock(this, fn->funcName, fn);
	m_BasicBlocks.emplace_back(bb);
	m_CurrentBB = bb;
	return bb;
}

//BasicBlock* ControlFlowGraph::CreateNewCurrBB(Function* fn)
//{
//	auto* bb = new BasicBlock(this, "BasicBlock_" + std::to_string(m_BasicBlocks.size()), fn);
//	m_BasicBlocks.emplace_back(bb);
//	m_CurrentBB = bb;
//	return bb;
//}

void ControlFlowGraph::GenerateX86(std::ostream& ostream)
{
	for (const auto* bb : m_BasicBlocks)
	{
		bb->GenerateX86(ostream);
	}
}
