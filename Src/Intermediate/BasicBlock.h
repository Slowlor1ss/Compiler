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
	//std::vector<Instruction*> GetInstrList() const { return m_Instructions; }
	void OptimizeIR();
	void GenerateX86(std::ostream& o);

	//TODO: maybe optimize this or add an replace current instruction as we often just replace the curr instruction and thus have the iterator in optimizeIR
	void ReplaceInstruction(Instruction* oldInst, Instruction* newInst)
	{
		std::ranges::replace(m_Instructions, oldInst, newInst);
		delete oldInst;
	}

	void SetExitTrue(BasicBlock* basicBlock) { m_ExitTrue = basicBlock; if(basicBlock) m_ExitTrue->SetEntryBlock(this); }
	void SetExitFalse(BasicBlock* basicBlock) { m_ExitFalse = basicBlock; if (basicBlock) m_ExitFalse->SetEntryBlock(this); }

	std::vector<BasicBlock*> GetEntryBlocks() { return m_Entries; }
	BasicBlock* GetExitTrue() const { return m_ExitTrue; }
	BasicBlock* GetExitFalse() const { return m_ExitFalse; }

	//std::optional<int> FindConst(const std::string& symName)
	//{
	//	if (const auto it = m_ConstTable.find(symName); it != m_ConstTable.end()) {
	//		return it->second.first;
	//	}
	//	return {};
	//}
	void AddConst(Symbol* sym, std::optional<int> value) { m_ConstTable[sym->varName] = std::pair(value, sym); }

	//bool GetAsmInitialized() const { return !m_ConstUninitilaized; }

private:
	void SetEntryBlock(BasicBlock* basicBlock) { m_Entries.emplace_back(basicBlock); }

	ControlFlowGraph* m_CFG;
	std::string m_Label;
	Function* m_Function;
	std::vector<Instruction*> m_Instructions;

	std::vector<BasicBlock*> m_Entries{};
	BasicBlock* m_ExitTrue{nullptr};
	BasicBlock* m_ExitFalse{nullptr};

	// The idea of this map is to hold for every block its constants (connects the name to the value but specific to that block)
	// we can use this to compare our const value with the parenting connections of this basic block
	// This is required to make sure we can get optimal const propagation as it is unsafe to just keep a variable const untill the next assignment
	// to give an example look at this psudo code:
	//
	// x=5;			\| bb1 (basic block 1)
	// y=x+5;		/|				bb1		bb2		(CFG)
	// if(unknown)	\| bb2			  \		/
	//   x=3;		/|					bb3
	// return x;	-| bb3
	//
	// we can know at compile time that y = 10; as we are sure of the value of x up until the if statement (and even inside the if statement were sure untill the assignment)
	// so to keep the const value of x as long as possible we sort of compare the value of x in this block to the prev one
	// for a more complete explanation
	// this algorithm is a modified implementation of the algorithm described here https://iitd.github.io/col728/lec/global_constant_propagation.html
	//
	// Basically holds a copy of the cost value in this basic block so that even if it changes later in another block here it stays the last const value it ahd in this block
	std::unordered_map<std::string, std::pair<std::optional<int>, Symbol*>> m_ConstTable{};

	bool m_ConstUninitilaized{ true };

	// TODO: maybe try and find a faster algorithm (But fist TEST)
	// Big O notation should be something like O(n*m),
	// where n is the number of maps in the input vector and m is the average number of elements in the maps
	//
	// Calculates the "IN" set by intersecting all the "OUT" sets of the predecessor blocks
	void ComputeInSet(const std::vector<BasicBlock*>& blocks)
	{
		// Intersect the maps
		for (const auto& block : blocks)
		{
			// If there's an uninitialized block we don't know if variables will change in that block,
			// so by default none will be const
			if (block->m_ConstUninitilaized)
			{
				auto maxMap = blocks[0];
				size_t maxSize = maxMap->m_ConstTable.size();
				for (size_t i{1}; i < blocks.size(); ++i)
				{
					const size_t size = blocks[i]->m_ConstTable.size();
					if(size > maxSize)
					{
						maxMap = blocks[i];
						maxSize = size;
					}
				}

				for (const auto & [key, value] : maxMap->m_ConstTable)
				{
					m_ConstTable[key].first = std::nullopt;
					m_ConstTable[key].second = value.second;
					// Also update the original symbol to be not const anymore
					m_ConstTable[key].second->constVal = std::nullopt;
				}
				return;
			}

			for (const auto& [key, value] : block->m_ConstTable)
			{
				if (!value.first) 
				{
					// If the value is empty, set the intersection value to empty
					m_ConstTable[key].first = std::nullopt;
				}
				else if (m_ConstTable.find(key) == m_ConstTable.end())
				{
					// If the key is not in the intersection map, add it
					m_ConstTable[key] = value;
					// Also update the original symbol
					m_ConstTable[key].second->constVal = value.first;
				}
				else if (m_ConstTable[key].first && *m_ConstTable[key].first != *value.first)
				{
					// If the key is in the intersection map and the values are different, set the intersection value to empty
					m_ConstTable[key].first = std::nullopt;
					// Also update the original symbol to be not const anymore
					m_ConstTable[key].second->constVal = std::nullopt;
				}
			}
		}
	}

};

