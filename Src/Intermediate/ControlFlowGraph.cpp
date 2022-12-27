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
//TODO: cleanup
//BasicBlock* ControlFlowGraph::CreateNewCurrBB(Function* fn)
//{
//	auto* bb = new BasicBlock(this, "BasicBlock_" + std::to_string(m_BasicBlocks.size()), fn);
//	m_BasicBlocks.emplace_back(bb);
//	m_CurrentBB = bb;
//	return bb;
//}

void ControlFlowGraph::GenerateX86(std::stringstream& ss)
{
	if (m_Optimized)
	{
		for (auto* bb : m_BasicBlocks)
		{
			bb->OptimizeIR();
		}
	}

	// The .text directive indicates the start of the code section of the program
	ss << ".text\n";

	for (const auto* bb : m_BasicBlocks)
	{
		bb->GenerateX86(ss);
	}

	//if (m_Optimized)
	//{
	//	OptimizeASM(ss);
	//}
}

void ControlFlowGraph::OptimizeASM(std::stringstream& ss)
{
	//TODO: finish
	std::vector<std::string> outputLines;

	std::string prevInstr = "";
	std::string prevDest = "";
	std::string prevSrc = "";

	std::string line;
	while (std::getline(ss, line)) 
	{
		// Split string in [instruction] and [param, param, ...]
		std::vector<std::string> args = Split(line, "\t");

		// If current instruction is an instruction without parameters -> continue
		if (args.size() < 2) 
		{
			outputLines.push_back(line);
			continue;
		}

		std::string currInstr = args[0];
		if (currInstr == "movl" || currInstr == "movb") 
		{
			// Get the individual arguments
			std::vector<std::string> params = Split(args[1], ", ");
			// If if the same instruction but with reversed operands than we can remove it as its useless
			if (currInstr == prevInstr && params[0] == prevDest && params[1] == prevSrc) 
			{
				outputLines.pop_back(); //TODO: test
				continue;
			}

			prevSrc = params[0];
			prevDest = params[1];
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
