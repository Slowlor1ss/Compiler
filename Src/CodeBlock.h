#pragma once
#include <list>
#include "Instruction.h"

class CodeBlock
{
	void AddInstr(Instruction::Operation op, std::vector<std::string> params, SymbolTable* sT);

	// Getters / Setters
	std::list<Instruction*> GetInstrList() { return m_InstrList; }

private:

	/* The instructions themselves */
	std::list<Instruction*> m_InstrList;
};

