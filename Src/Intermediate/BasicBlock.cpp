#include "BasicBlock.h"
#include "ControlFlowGraph.h"
#include "Instruction.h"
#include "../Scope.h"

BasicBlock::BasicBlock(ControlFlowGraph* cfg, std::string label, Function* fn)
	: m_CFG(cfg)
	, m_Label(std::move(label))
	, m_Function(fn)
{
}

BasicBlock::~BasicBlock()
{
	for (auto block : m_Entries)
	{
		if (block->m_ExitTrue == this)
		{
			block->m_ExitTrue = nullptr;
		}
		if (block->m_ExitFalse == this)
		{
			block->m_ExitFalse = nullptr;
		}
	}
	if (m_ExitTrue)
	{
		std::erase(m_ExitTrue->m_Entries, this);
	}
	if (m_ExitFalse)
	{
		std::erase(m_ExitFalse->m_Entries, this);
	}

	for (auto* instruction : m_Instructions)
	{
		delete instruction;
		instruction = nullptr;
	}
}

//void BasicBlock::AddInstr(Instruction::Operation op, std::string dest, std::vector<std::string> params, Scope* sT)
//{
//	m_Instructions.emplace_back(new Instruction(this, op, dest, std::move(params), sT));
//}

void BasicBlock::AddInstr(Instruction* instr)
{
	m_Instructions.emplace_back(instr)->SetBasicBlock(this);
}

void BasicBlock::OptimizeIR()
{
	if constexpr (g_OptimizeConstPropagation)
	{
		//compute in set
		ComputeInSet(m_Entries);

		for (auto i = m_Instructions.begin(); i != m_Instructions.end();)
		{
			if ((*i)->PropagateConst())
				i = m_Instructions.erase(i);
			else
				++i;
		}

		m_ConstUninitilaized = false;
	}
}

void BasicBlock::GenerateX86(std::ostream& o)
{
	if (m_Label[0] == '.')
	{
		o << m_Label << ":\n";
	}
	for (auto* instr : m_Instructions)
		instr->GenerateASM(o);
}

void BasicBlock::ReplaceInstruction(Instruction* oldInst, Instruction* newInst)
{
	std::ranges::replace(m_Instructions, oldInst, newInst);
	delete oldInst;
}

std::optional<int> BasicBlock::GetConst(const Symbol* sym)
{
	if constexpr (g_RemoveDeadcode)
	{
		if (const auto it = m_ConstTable.find(sym->varName); it != m_ConstTable.end()) {
			return it->second.GetValue(sym->varName);
		}
	}
	return sym->constVal;
}

void BasicBlock::AddAndUpdateConst(Symbol* sym, std::optional<int> value, Operation::WriteConst* writeConstInst)
{
	if (m_ConstTable.contains(sym->varName))
	{
		m_ConstTable[sym->varName] = ConstPropInfo{ value, sym, writeConstInst, this, !value.has_value() };//false//prev true && has_value };
	}
	else //if(value.has_value()) //first time adding this const (if it never had a const value to begin with no need to add it)
	{
		m_ConstTable.emplace(sym->varName, ConstPropInfo{ value, sym, writeConstInst, this, !value.has_value() });
		//TODO: remove below maybe
		// If it has no value from the start that means it never was const and thus has a write instruction
		//m_ConstTable[sym->varName].m_HasWriteInstr = !value.has_value();
	}
}

void BasicBlock::RemoveEntryBlock(BasicBlock* basicBlock)
{
	std::erase(m_Entries, basicBlock);
}

void BasicBlock::ComputeInSet(const std::vector<BasicBlock*>& blocks)
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
			for (size_t i{ 1 }; i < blocks.size(); ++i)
			{
				const size_t size = blocks[i]->m_ConstTable.size();
				if (size > maxSize)
				{
					maxMap = blocks[i];
					maxSize = size;
				}
			}

			for (const auto& [key, value] : maxMap->m_ConstTable)
			{
				//m_ConstTable.emplace(key, ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, value.m_BB });
				m_ConstTable[key] = ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, this, false }; //TODO: check if false is good here
				//m_ConstTable.emplace(key, ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, value.m_BB });
				//m_ConstTable[key].first = std::nullopt;
				//m_ConstTable[key].second = value.second;
				//// Also update the original symbol to be not const anymore
				//m_ConstTable[key].second->constVal = std::nullopt;
			}
			return;
		}

		for (const auto& [key, value] : block->m_ConstTable)
		{
			if (!value.m_Value)
			{
				// If the value is empty, set the intersection value to empty
				if (m_ConstTable.contains(key) && !m_ConstTable[key].m_HasWriteInstr)
				{
					m_ConstTable[key] = ConstPropInfo{ value.m_Value, value.m_OriginalSym, value.m_WriteConstInst, this, false };
				}
				else
				{
					m_ConstTable[key] = ConstPropInfo{ value.m_Value, value.m_OriginalSym, value.m_WriteConstInst, this, value.m_HasWriteInstr };
				}
				//m_ConstTable.emplace(key, value);
			}
			else if (m_ConstTable.find(key) == m_ConstTable.end())
			{
				// If the key is not in the intersection map, add it
				m_ConstTable.emplace(key, ConstPropInfo{ value.m_Value, value.m_OriginalSym, value.m_WriteConstInst, this, value.m_HasWriteInstr });
				//m_ConstTable[key] = value;
			}
			else if (m_ConstTable[key].m_Value && *m_ConstTable[key].m_Value != *value.m_Value)
			{
				// If the key is in the intersection map and the values are different, set the intersection value to empty
				//m_ConstTable.emplace(key, ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, value.m_BB });
				m_ConstTable[key] = ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, this, false };
			}
		}
	}
}

ConstPropInfo::ConstPropInfo(std::optional<int> value, Symbol* origSym, Operation::WriteConst* writeConstInst,
	BasicBlock* bb, bool hasWriteInst)
	: m_BB(bb), m_OriginalSym(origSym), m_WriteConstInst(writeConstInst), m_Value(value), m_HasWriteInstr(hasWriteInst)
{
	m_OriginalSym->constVal = value;
	//m_OriginalSym->constInfo = m_BB->GetConstTable().at(0).GetValue("");
}

//ConstPropInfo& ConstPropInfo::operator=(const ConstPropInfo& c)
//{
//	m_Value = c.m_Value;
//	m_OriginalSym = c.m_OriginalSym;
//	m_WriteConstInst = c.m_WriteConstInst;
//	m_BB = c.m_BB;
//	m_HasWriteInstr = c.m_HasWriteInstr;
//	return this;
//}

ConstPropInfo::ConstPropInfo(const ConstPropInfo& c)
	: ConstPropInfo(c.m_Value, c.m_OriginalSym, c.m_WriteConstInst, c.m_BB, c.m_HasWriteInstr)
{
	m_OriginalSym->constVal = c.m_Value;
	//m_OriginalSym->constInfo = this;
}


//void ConstPropInfo::SetValue(std::optional<int> value, Symbol* origSym, Operation::WriteConst* writeConstInst)
//{
//	{
//		//if (m_Value.has_value())
//		//{
//		//	// Makes sure only the last use inside the block will be written
//		//	delete m_WriteConstInst;
//		//	m_WriteConstInst = nullptr;
//		//}
//
//		m_Value = value;
//		if (m_Value.has_value())
//			m_HasWriteInstr = false;
//		m_OriginalSym = origSym;
//		// Also update original value
//		m_OriginalSym->constVal = value;
//		m_OriginalSym->constInfo = this;
//		m_WriteConstInst = writeConstInst;
//	}
//}

std::optional<int> ConstPropInfo::GetValue(const std::string& name)
{
	if (!m_Value.has_value())
	{
		if (m_HasWriteInstr)
			return {};

		// If we get here that means that the code is being used when its not a const anymore so we have to make sure there
		// are instructions that write the latest value
		for (const auto& entry : m_BB->GetEntryBlocks())
		{
			//TODO: TEST FOR X NOT COST AND THEN LATER SET TO CONST AND THEN BACK TO NOT CONST SHOULD WRITE AN INSTRUCTION!
			auto& constTable = entry->GetConstTable();
			if (const auto it = constTable.find(name); it != constTable.end()) {
				auto& c = it->second;
				if (!c.m_HasWriteInstr) // Has the latest version of this const already been written to the right memory location?
				{
					c.m_WriteConstInst->SetUsed(true);
				}
				else
				{
#ifdef _DEBUG
					__debugbreak();
#endif
				}
			}
		}
		m_HasWriteInstr = true;
		return {};
	}
	else
	{
		return m_Value;
	}
}
