#include "ControlFlowGraph.h"
#include "BasicBlock.h"
#include "../Scope.h"
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <support/CPPUtils.h>
#include "Instruction.h"
#include "../SymbolTable.h"
#include "../CompilerFlags.hpp"

ControlFlowGraph::ControlFlowGraph(bool optimized)
	: m_Optimized(optimized)
{
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
	auto* bb = new BasicBlock(this, fn->funcName, fn);
	m_BasicBlocks.emplace_back(bb);
	m_CurrentBB = bb;
	return bb;
}

BasicBlock* ControlFlowGraph::CreateNewStatementBB(Function* fn)
{
	auto* bb = new BasicBlock(this, ".B" + std::to_string(m_BasicBlocks.size()), fn);
	m_BasicBlocks.emplace_back(bb);
	return bb;
}

void ControlFlowGraph::RemoveBasicBlock(std::string label)
{
	// Find the first object in the vector with a matching name
	auto it = std::ranges::find_if(m_BasicBlocks, [&](const BasicBlock* obj) {
		return obj->GetLabel() == label;
		});
	//TODO: we can also erase if not looping over the container
	// Return a pointer to the object if it was found, otherwise return nullptr
	if (it != m_BasicBlocks.end())
	{
		delete *it;
		*it = nullptr;
	}
}

void ControlFlowGraph::GenerateX86(std::stringstream& ss)
{
	for (auto i = m_BasicBlocks.begin(); i != m_BasicBlocks.end();)
	{
		if ((*i) == nullptr)
			i = m_BasicBlocks.erase(i);
		else
		{
			(*i)->OptimizeIR();
			++i;
		}
	}

	if(compilerFlags::g_RemoveDeadcode)
	{
		for (const auto& m_BasicBlock : std::ranges::reverse_view(m_BasicBlocks))
		{
			m_BasicBlock->DCE(true);
		}
	}
	//for (const auto & basicBlock : m_BasicBlocks)
	//{
	//	basicBlock->DCE(true);
	//}

	//std::erase(m_BasicBlocks, nullptr);

	// The .text directive indicates the start of the code section of the program
	ss << ".text\n";

	for (auto* bb : m_BasicBlocks)
	{
		bb->GenerateX86(ss);
	}

	if (compilerFlags::g_PeepholeOptimization)
	{
		PeepholeOptimization(ss);
	}
}

void ControlFlowGraph::SetSymbolTable(SymbolTable* st)
{
	m_GlobalSymbolTable = st;
}

//TODO:!!! clean up put in to functions
void ControlFlowGraph::PeepholeOptimization(std::stringstream& ss)
{
	//TODO: finish
	std::vector<std::string> outputLines;

	std::string prevInstr = "";
	std::string prevDest = "";
	std::string prevSrc = "";

	std::string currFunction = "";
	std::string prevStackSize{};
	int funcStackPrologueIdx{ -1 };
	int spillNr{ 0 };
	std::unordered_map<std::string, std::string> spillMap{};

	//TODO: optimize conditionals and don't forget to optimize the wile like gcc does

	std::string line;
	while (std::getline(ss, line)) 
	{
		// Split string in [instruction] and [param, param, ...]
		std::vector<std::string> args = Split(line, "\t");

		// If current instruction is an instruction without parameters -> continue
		if (args.size() < 2) 
		{
			// Remove unnecessary jumps
			if (args[0][0] == '.') //basic block labels always start with a .B
			{
				// If it corresponds to the jump previously visited
				std::string::size_type pos = args[0].find(':');
				if (args[0].substr(0, pos) == prevDest)
					outputLines.pop_back(); // Remove jump
				else
					prevDest = "";
			}
			else
			{
				// If it has a : its a label and if it didn't start with a . its not a basic block so must be a function
				std::string::size_type pos = args[0].find(':');
				if (pos != std::string::npos)
				{
					currFunction = args[0].substr(0, pos);
					funcStackPrologueIdx = -1;
				}
			}

			outputLines.push_back(line);
			continue;
		}

		std::string currInstr = args[0];

		// Get the individual arguments
		std::vector<std::string> params = Split(args[1], ", ");
		if (params.size() > 1)
		{
			// Remove temp vars
			if (compilerFlags::g_RemoveTempVars)
			{
			//NOTE!: this is an extremely bad way of doing this but due to the need of a very big design change for proper register allocation
			//		 and a lack of time I will for now leave it with a wacky implementation of conservative spilling
			for (const auto & param : params)
			{
				if (param[0] == '[')
				{
					if (spillMap.contains(param))
					{
						line = ReplaceString(line, param, spillMap[param]);

						if (compilerFlags::g_AddComents)
						{
						//Update comment
						std::vector<std::string> lineComment = Split(line, "#");
						if (lineComment.size()>1)
						{
							line = ReplaceString(line, lineComment[1], " restore spill nr"+std::to_string(spillNr));
						}
						}
					}
				}
			}

			if (prevDest[0] == '[')
			{
				//NOTE!: this is an extremely bad way of doing this but due to the need of a very big design change for proper register allocation
				//		 and a lack of time I will for now leave it with a wacky implementation of conservative spilling
				std::string::size_type p = line.find(prevDest);
				if (p == std::string::npos)
				{
					//Need to spill
					std::string spillPos;
					auto* func = m_GlobalSymbolTable->GetGlobalScope()->GetFunc(currFunction);
					char opSize = *(prevInstr.end() - 1);
					Symbol* spillSym = func->scope->AddSymbol("spill" + std::to_string(spillNr), opSize == 'l' ? "int" : "char", 0);
					++spillNr;
					spillMap[prevDest] = spillSym->GetOffsetReg();
					std::string correctedPrevLine = ReplaceString(outputLines.back(), prevDest, spillSym->GetOffsetReg());
					//Update comment
					if (compilerFlags::g_AddComents)
					{
					//Update comment
					std::vector<std::string> lineComment = Split(correctedPrevLine, "#");
					if (lineComment.size() > 1)
					{
						correctedPrevLine = ReplaceString(correctedPrevLine, lineComment[1], " spilling nr" + std::to_string(spillNr));
					}
					}
					outputLines.back() = correctedPrevLine; // Update memory location

					if (funcStackPrologueIdx != -1)
					{
						int newSize = Instruction::RoundUpToMultipleOf16(func->scope->GetScopeSize());
						std::string newSizeStr = '$' + std::to_string(newSize);
						if (newSizeStr != prevStackSize)
						{
							prevStackSize = newSizeStr;
							std::string prologueLine = outputLines[funcStackPrologueIdx];
							std::vector<std::string> args2 = Split(prologueLine, "\t");
							std::vector<std::string> params2 = Split(args2[1], ", ");
							prologueLine = ReplaceString(prologueLine, params2[0], '$' + std::to_string(newSize));
							outputLines[funcStackPrologueIdx] = prologueLine;
						}
					}

					// Continue as normal
					args = Split(line, "\t");
					params = Split(args[1], ", ");

					prevSrc = params[0];
					prevDest = params[1];

					prevInstr = currInstr;
					outputLines.push_back(line);
					continue;
				}

				outputLines.pop_back();

				line = ReplaceString(line, prevDest, prevSrc);

				args = Split(line, "\t");
				params = Split(args[1], ", ");

				prevSrc = params[0];
				prevDest = params[1];

				if (	(params[0]==params[1])
					||	(params[0] == "%eax" && params[1] == "%al")
					||	(params[0] == "%al" && params[1] == "%eax")
																	)
					continue;

				prevInstr = currInstr;
				outputLines.push_back(line);
				continue;
			}
			}

			if (currInstr == "movl" || currInstr == "movb") 
			{
				// If if the same instruction but with reversed operands than we can remove it as its useless // I know this is insane
				if ((params[0] == prevDest && (params[1] == prevSrc || params[1] == "%al" && prevSrc == "%eax" || params[1] == "%eax" && prevSrc == "%al")) &&
					(currInstr == prevInstr || currInstr == "movl" && prevInstr == "movb" || currInstr == "movb" && prevInstr == "movl"))
				{
					//outputLines.pop_back(); //TODO: test
					continue;
				}
			}

			if (currInstr == "subq")
			{
				if (params[1] == "%rbp")
				{
					funcStackPrologueIdx = outputLines.size();
					prevStackSize = params[0];
				}
			}

			prevSrc = params[0];
			prevDest = params[1];
		}

		if(currInstr == "jmp")
		{
			// Get the jump location
			std::string jumpDest = Split(args[1], ", ")[0];
			if (jumpDest == prevDest)
				continue;
			prevDest = jumpDest;
		}


		prevInstr = currInstr;
		outputLines.push_back(line);
	}

	ss.clear();
	for (const std::string& stringLine : outputLines) {
		ss << stringLine << '\n';
	}
}

std::vector<std::string> ControlFlowGraph::Split(const std::string& s, const std::string& sep) {
	std::vector<std::string> parts;
	std::string ss = s;

	if (s.empty())
		return parts;

	std::string::size_type p = ss.find(sep);
	while (!ss.empty() && p != std::string::npos) 
	{
		if (std::string part = ss.substr(0, p); !part.empty()) // Don't allow empty strings
			parts.push_back(part);

		ss = ss.substr(p + sep.size());

		p = ss.find(sep);
	}
	parts.push_back(ss);

	return parts;
}

std::string ControlFlowGraph::ReplaceString(const std::string& s, const std::string& from, const std::string& to)
{
	std::string res;

	std::string ss = s;
	std::string::size_type p = ss.find(from);
	while (p != std::string::npos) {
		if (p > 0)
			res.append(ss.substr(0, p)).append(to);
		else
			res.append(to);
		ss = ss.substr(p + from.size());
		p = ss.find(from);
	}
	res.append(ss);

	return res;
}
