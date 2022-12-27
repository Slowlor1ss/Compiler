#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "Instruction.h"

struct Function;
class Scope;
class ControlFlowGraph;
class Instruction;

/// <summary>
/// Represents a node from the control-flow graph
///	https://en.wikipedia.org/wiki/Basic_block
/// </summary>
class BasicBlock
{
public:
	BasicBlock(ControlFlowGraph* cfg, std::string label, Function* fn);
	~BasicBlock();

	//void AddInstr(Instruction::Operation op, std::string dest, std::vector<std::string> params, Scope* sT);
	void AddInstr(Instruction* instr);

	ControlFlowGraph* GetCFG() const { return m_CFG; }
	std::string GetLabel() const { return m_Label; }
	Function* GetFunction() const { return m_Function; }
	std::vector<Instruction*> GetInstrList() const { return m_Instructions; }
	bool EvaluateConst(std::vector<Instruction*>::iterator it);
	void OptimizeIR();
	void GenerateX86(std::ostream& o) const;

	void ReplaceInstruction(Instruction* oldInst, Instruction* newInst)
	{
		std::ranges::replace(m_Instructions, oldInst, newInst);
		delete oldInst;
	}
private:
	ControlFlowGraph* m_CFG;
	std::string m_Label;
	Function* m_Function;
	std::vector<Instruction*> m_Instructions;

};

