#pragma once
#include <string>
#include <vector>
#include "Instruction.h"

class Scope;
class ControlFlowGraph;
class Instruction;

/// <summary>
/// Represents a node from the control-flow graph
/// </summary>
class BasicBlock
{
public:
	BasicBlock(ControlFlowGraph* cfg, std::string label);
	~BasicBlock();

	void AddInstr(Instruction::Operation op, std::string dest, std::vector<std::string> params, Scope* sT);
	void AddInstr(Instruction* instr);

private:
	ControlFlowGraph* m_CFG;
	std::string m_Label;
	std::vector<Instruction*> m_Instructions;

};

