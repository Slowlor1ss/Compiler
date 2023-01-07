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

// Note to self: if ever improving or remaking a compiler consider using defChains
// see paper "A SURVEY OF DATA FLOW ANALYSIS TECHNIQUES" by Ken Kennedy
void BasicBlock::DCE(std::vector<Instruction*>& worklist)
{
	for (auto* instruction : m_Instructions)
	{
		//switch (instruction->GetOp())
		//{
		//case Instruction::OperationType::Prologue: break;
		//case Instruction::OperationType::WriteConst: break;
		//case Instruction::OperationType::ReadParam: break;
		//case Instruction::OperationType::WriteParam: break;
		//case Instruction::OperationType::Call: break;
		//case Instruction::OperationType::Return: 
		//	break;
		//case Instruction::OperationType::Declaration: break;
		//case Instruction::OperationType::Assign: 
		//	break;
		//case Instruction::OperationType::LessThan:
		//case Instruction::OperationType::GreaterThan:
		//case Instruction::OperationType::BitwiseAnd:
		//case Instruction::OperationType::BitwiseOr:
		//case Instruction::OperationType::BitwiseXor:
		//case Instruction::OperationType::Equal:
		//case Instruction::OperationType::NotEqual:
		//case Instruction::OperationType::LessOrEqual:
		//case Instruction::OperationType::GreaterOrEqual: 
		//case Instruction::OperationType::Plus:
		//case Instruction::OperationType::Minus:
		//case Instruction::OperationType::Mul:
		//case Instruction::OperationType::Div:
		//case Instruction::OperationType::Mod:
		//	auto* inst = dynamic_cast<Operation::TwoOperandInstruction*>(instruction);
		//	if (inst->GetResult()->isUsed)
		//	{
		//		//remove inst from worklist

		//		if (!inst->GetLhs()->isUsed)
		//		{
		//			inst->GetLhs()->isUsed = true;
		//			//add to worklist

		//		}

		//		if (!inst->GetRhs()->isUsed)
		//		{
		//			inst->GetRhs()->isUsed = true;
		//			//add to worklist

		//		}


		//	}
		//	break;
		//case Instruction::OperationType::PlusEqual:
		//case Instruction::OperationType::MinusEqual:
		//case Instruction::OperationType::MulEqual:
		//case Instruction::OperationType::DivEqual:

		//	break;

		//case Instruction::OperationType::ExclamationMark:
		//case Instruction::OperationType::Negate:
		//	break;

		//case Instruction::OperationType::ConditionalJump: break;
		//case Instruction::OperationType::UnconditionalJump: break;
		//default: ;
		//}
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
	if (writeConstInst)
		m_ConstTable[sym->varName] = ConstPropInfo{ value, sym, {writeConstInst}, this};
	else
		m_ConstTable[sym->varName] = ConstPropInfo{ value, sym, {}, this};
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
				m_ConstTable[key] = ConstPropInfo{ std::nullopt, value.m_OriginalSym, value.m_WriteConstInst, this};
			}
			return;
		}

		for (const auto& [key, value] : block->m_ConstTable)
		{
			if (!value.m_Value)
			{
				// If the value is empty, set the intersection value to empty
				if (m_ConstTable.contains(key))
				{
					m_ConstTable[key].m_WriteConstInst.insert(m_ConstTable[key].m_WriteConstInst.end(), value.m_WriteConstInst.begin(), value.m_WriteConstInst.end());
					m_ConstTable[key].m_Value = std::nullopt;
				}
				else
				{
					m_ConstTable.emplace(key, ConstPropInfo{ value.m_Value, value.m_OriginalSym, value.m_WriteConstInst, this});
				}
			}
			else if (!m_ConstTable.contains(key))
			{
				// If the key is not in the intersection map, add it
				m_ConstTable.emplace(key, ConstPropInfo{ value.m_Value, value.m_OriginalSym, value.m_WriteConstInst, this});
			}
			else
			{
				if (m_ConstTable[key].m_Value && *m_ConstTable[key].m_Value != *value.m_Value)
				{
					// If the key is in the intersection map and the values are different, set the intersection value to empty
					m_ConstTable[key].m_Value = std::nullopt;
				}
				// Add all write instructions
				m_ConstTable[key].m_WriteConstInst.insert(m_ConstTable[key].m_WriteConstInst.end(), value.m_WriteConstInst.begin(), value.m_WriteConstInst.end());
			}
		}
	}
}

ConstPropInfo::ConstPropInfo(std::optional<int> value, Symbol* origSym, std::vector<Operation::WriteConst*> writeConstInst,
	BasicBlock* bb)
	: m_BB(bb), m_OriginalSym(origSym), m_WriteConstInst(std::move(writeConstInst)), m_Value(value)
{
	m_OriginalSym->constVal = value;
}

ConstPropInfo::ConstPropInfo(const ConstPropInfo& c)
	: ConstPropInfo(c.m_Value, c.m_OriginalSym, c.m_WriteConstInst, c.m_BB)
{
	m_OriginalSym->constVal = c.m_Value;
}

std::optional<int> ConstPropInfo::GetValue(const std::string& name)
{
	if (!m_Value.has_value())
	{
		if (m_WriteConstInst.empty())
			return {};

		// If we get here that means that the code is being used when its not a const anymore so we have to make sure there
		// are instructions that write the latest value
		for (auto* writeConstInst : m_WriteConstInst)
		{
			writeConstInst->SetUsed(true);
		}
		m_WriteConstInst.clear();

		return {};
	}
	else
	{
		return m_Value;
	}
}
