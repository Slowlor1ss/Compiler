#pragma once
#include <vector>
class Scope;
class BasicBlock;

class ControlFlowGraph
{
public:
	ControlFlowGraph(bool optimized = false);
	~ControlFlowGraph();

	BasicBlock* CreateNewCurrBB();

	bool GetOptimized() { return m_Optimized; }
	void SetOptimized(bool o) { m_Optimized = o; }
	BasicBlock* CurrentBB() const { return m_CurrentBB; }
	void SetCurrentBB(BasicBlock* bb) { m_CurrentBB = bb; }

private:
	
	std::vector<BasicBlock*> m_BasicBlocks;
	//Option to enable CFG optimizations
	bool m_Optimized;
	BasicBlock* m_CurrentBB{};

};

