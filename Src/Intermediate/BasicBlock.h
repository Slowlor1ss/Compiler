#pragma once
#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
//#include "../Scope.h"


class BasicBlock;

namespace Operation
{
	class WriteConst;
}

struct Symbol;
class ConstPropInfo;
struct Function;
class Scope;
class ControlFlowGraph;
class Instruction;

class ConstPropInfo
{
public:
	ConstPropInfo() = default; //set things liek in other constructor body
	ConstPropInfo(std::optional<int> value, Symbol* origSym, std::vector<Operation::WriteConst*> writeConstInst, BasicBlock* bb);
	ConstPropInfo(const ConstPropInfo& c);

	std::optional<int> GetValue(const std::string& name);

private:
	friend class BasicBlock;

	BasicBlock* m_BB{ nullptr };
	Symbol* m_OriginalSym{ nullptr };
	std::vector<Operation::WriteConst*> m_WriteConstInst{ nullptr };
	std::optional<int> m_Value{};
};

/// <summary>
/// Represents a node from the control-flow graph
///	https://en.wikipedia.org/wiki/Basic_block
/// </summary>
class BasicBlock
{
public:
	BasicBlock(ControlFlowGraph* cfg, std::string label, Function* fn);
	~BasicBlock();

	void AddInstr(Instruction* instr);

	ControlFlowGraph* GetCFG() const { return m_CFG; }
	std::string GetLabel() const { return m_Label; }
	Function* GetFunction() const { return m_Function; }

	void OptimizeIR();
	void DCE(bool allowSetUnused);
	void GenerateX86(std::ostream& o);

	//TODO: maybe optimize this or add an replace current instruction as we often just replace the curr instruction and thus have the iterator in optimizeIR
	void ReplaceInstruction(Instruction* oldInst, Instruction* newInst);

	void SetExitTrue(BasicBlock* basicBlock) { m_ExitTrue = basicBlock; if(basicBlock) m_ExitTrue->SetEntryBlock(this); }
	void SetExitFalse(BasicBlock* basicBlock) { m_ExitFalse = basicBlock; if (basicBlock) m_ExitFalse->SetEntryBlock(this); }

	std::vector<BasicBlock*> GetEntryBlocks() { return m_Entries; }
	BasicBlock* GetExitTrue() const { return m_ExitTrue; }
	BasicBlock* GetExitFalse() const { return m_ExitFalse; }

	std::optional<int> GetConst(const Symbol* sym);

	const auto& GetConstTable() { return m_ConstTable; }
	void AddAndUpdateConst(Symbol* sym, std::optional<int> value, Operation::WriteConst* writeConstInst);
	void RemoveEntryBlock(BasicBlock* basicBlock);

private:
	void SetEntryBlock(BasicBlock* basicBlock) { m_Entries.emplace_back(basicBlock); }

	bool m_Used{false};

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
	// we can know at compile time that y = 10; as we are sure of the value of x up until the if statement (and even inside the if statement were sure until the assignment)
	// so to keep the const value of x as long as possible we compare the value of x in this block to the prev one if its the same we can keep it, else its unknown
	// for a more complete explanation:
	// my algorithm is a modified implementation of the algorithm described here https://iitd.github.io/col728/lec/global_constant_propagation.html
	//
	// Basically holds a copy of the cost value in this basic block so that even if it changes later in another block here it stays the last const value it ahd in this block
	std::unordered_map<std::string, ConstPropInfo> m_ConstTable{};

	bool m_ConstUninitilaized{ true };

	// Big O notation should be something like O(n*m),
	// where n is the number of maps in the input vector and m is the average number of elements in the maps
	//
	// Calculates the "IN" set by intersecting all the "OUT" sets of the predecessor blocks
	void ComputeInSet(const std::vector<BasicBlock*>& blocks);
};
