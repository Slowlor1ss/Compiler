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
	ControlFlowGraph(bool optimized = false);
	~ControlFlowGraph();

	BasicBlock* CreateNewCurrBB(Function* fn);
	BasicBlock* CreateNewStatementBB(Function* fn);

	void RemoveBasicBlock(std::string label);

	bool GetOptimized() const { return m_Optimized; }
	void SetOptimized(bool o) { m_Optimized = o; }
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

//TODO: remove these globals and make some settings
//constexpr bool g_OptimizeConstPropagation{ true };
//constexpr bool g_RemoveDeadcode{ true };
//constexpr bool g_RemoveConstConditionals{ false };
//constexpr bool g_OptimizeASM{ true };
//constexpr bool g_RemoveTempVars{ true && g_OptimizeASM };
//constexpr bool g_AddComents{ true };
