#pragma once
#include <string>
#include <vector>
class SymbolTable;
struct Function;
class Scope;
class BasicBlock;

class ControlFlowGraph
{
public:
	ControlFlowGraph();
	~ControlFlowGraph();

	BasicBlock* CreateNewCurrBB(Function* fn);
	BasicBlock* CreateNewStatementBB(Function* fn);

	void RemoveBasicBlock(std::string label);

	BasicBlock* CurrentBB() const { return m_CurrentBB; }
	void SetCurrentBB(BasicBlock* bb) { m_CurrentBB = bb; }
	void GenerateX86(std::stringstream& ss);

	void SetSymbolTable(SymbolTable* st);

private:
	void PeepholeOptimization(std::stringstream& ss);
	std::vector<std::string> Split(const std::string& s, const std::string& sep);
	[[nodiscard]] std::string ReplaceString(const std::string& s, const std::string& from, const std::string& to);

	std::vector<BasicBlock*> m_BasicBlocks;
	//Option to enable CFG optimizations
	bool m_Optimized;
	BasicBlock* m_CurrentBB{};
	SymbolTable* m_GlobalSymbolTable{};
};
