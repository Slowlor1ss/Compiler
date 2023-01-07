#include "ControlFlowGraph.h"
#include "BasicBlock.h"
#include "../Scope.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <support/CPPUtils.h>

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

	//std::erase(m_BasicBlocks, nullptr);

	// The .text directive indicates the start of the code section of the program
	ss << ".text\n";

	for (auto* bb : m_BasicBlocks)
	{
		bb->GenerateX86(ss);
	}

	if (g_OptimizeASM)
	{
		OptimizeASM(ss);
	}
}

void ControlFlowGraph::OptimizeASM(std::stringstream& ss)
{
	//TODO: finish
	std::vector<std::string> outputLines;

	std::string prevInstr = "";
	std::string prevDest = "";
	std::string prevSrc = "";

	//TODO: optimize conditionals and don't forget to optimize the wile like gcc does

	std::string line;
	while (std::getline(ss, line)) 
	{
		// Split string in [instruction] and [param, param, ...]
		std::vector<std::string> args = Split(line, "\t");

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

		// If current instruction is an instruction without parameters -> continue
		if (args.size() < 2) 
		{
			outputLines.push_back(line);
			continue;
		}

		std::string currInstr = args[0];

		std::vector<std::string> params = Split(args[1], ", ");
		if (params.size() > 1)
		{
			// Remove temp vars
			if constexpr  (g_RemoveTempVars)
			{
			if (prevDest == "0(%rbp)")
			{
				outputLines.pop_back();

				line = ReplaceString(line, "0(%rbp)", prevSrc);

				args = Split(line, "\t");
				params = Split(args[1], ", ");

				prevSrc = params[0];
				prevDest = params[1];

				if (params[0]==params[1])
					continue;

				prevInstr = currInstr;
				outputLines.push_back(line);
				continue;
			}
			}

			if (currInstr == "movl" || currInstr == "movb") 
			{
				// Get the individual arguments
				std::vector<std::string> params = Split(args[1], ", ");
				// If if the same instruction but with reversed operands than we can remove it as its useless
				if (currInstr == prevInstr && params[0] == prevDest && params[1] == prevSrc) 
				{
					//outputLines.pop_back(); //TODO: test
					continue;
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
