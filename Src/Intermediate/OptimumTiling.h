#pragma once
#include <functional>
#include <optional>
#include "Instruction.h"
#include "../Scope.h"
#include <cassert>
#include "ControlFlowGraph.h"
#include "../CompilerFlags.hpp"

struct Symbol;

namespace InstrSelection
{
	enum PatternInst
	{
		// Order matters do not change this!
		loadl,
		loadb,
		loadbl,
		loadc,
		noloadl,
		noloadb,
	};

	enum PatternOp
	{
		plus,
		min,
		mul,
		div
	};

	struct Pattern
	{
		Pattern(PatternInst i, float cost, std::function<bool(Symbol*)> pred)
			: predicate(std::move(pred))
			, instr(i)
			, cost(cost)
		{
		}

		bool Match(Symbol* sym) const
		{
			return predicate(sym);
		}

		std::function<bool(Symbol*)> predicate;
		PatternInst instr;
		float cost;
	};

	class OptimumInstructionTiling
	{
	private:


	public:
		OptimumInstructionTiling(Symbol* lhs, Symbol* rhs, bool lhsIsConst, std::optional<int> constval, InstrSelection::PatternOp op);

		//																											(32 bit, 8 bit) regs
		std::array<std::string, 2> LoadReg(std::ostream& o, std::vector<std::pair<std::string, std::string>> regs = {{"%eax","%al"}, {"%edx","%dl"} })
		{
			assert(!(symbols.size() > regs.size()));

			// When using 2 registers its best to first load edx and then eax so result will be stored in eax
			if (inst[0] < 3 && inst[1] < 3)
				std::swap(regs[0], regs[1]);

			int regsIdx = 0;
			std::array<std::string, 2> params{};

			for (size_t i{ 0 }; i < symbols.size(); ++i)
			{
				switch (inst[i])
				{
				case loadl:
					m_OpSize = 32;
					params[i] = regs[regsIdx].first;
					o << Instruction::FormatInstruction("movl", symbols[i]->GetOffsetReg(), regs[regsIdx].first); Instruction::AddCommentToPrevInstruction(o, "["+OPToString(m_Op)+"] move " + symbols[i]->varName + " into " + regs[regsIdx].first);
					++regsIdx;
					break;
				case loadb:
					m_OpSize = 8;
					params[i] = regs[regsIdx].second;
					o << Instruction::FormatInstruction("movb", symbols[i]->GetOffsetReg(), regs[regsIdx].second); Instruction::AddCommentToPrevInstruction(o, "["+OPToString(m_Op)+"] move " + symbols[i]->varName + " into " + regs[regsIdx].second);
					++regsIdx;
					break;
				case loadbl:
					m_OpSize = 32;
					params[i] = regs[regsIdx].first;
					o << Instruction::FormatInstruction("movsbl", symbols[i]->GetOffsetReg(), regs[regsIdx].first); Instruction::AddCommentToPrevInstruction(o, "["+OPToString(m_Op)+"] move " + symbols[i]->varName + " into " + regs[regsIdx].first);
					++regsIdx;
					break;
				case loadc:
					params[i] = '$' + std::to_string(symbols[i]->constVal.value());
					break;
				case noloadb:
					m_OpSize = 8;
					params[i] = symbols[i]->GetOffsetReg();
					break;
				case noloadl:
					m_OpSize = 32;
					params[i] = symbols[i]->GetOffsetReg();
					break;
				}
			}

			return params;
		}
		void LoadRes(std::ostream& o);

		int GetOpsize() const { return m_OpSize; }

	private:
		std::array<Symbol*, 2> symbols{};
		std::array<PatternInst, 2> inst{};
		PatternOp m_Op;
		int m_OpSize{};

		//cost comes from https://agner.org/optimize/ based on AMD k7 machine
		inline static const std::array<Pattern, 6> m_Patterns{
			Pattern{loadl,	3,	[](const Symbol* sym) -> bool {return sym->varType == "int"; }},
			Pattern{loadb,	4,	[](const Symbol* sym) -> bool {return sym->varType == "char"; }},
			Pattern{loadbl,	4,	[](const Symbol* sym) -> bool {return (!sym->constVal.has_value() && sym->varType == "char"); }},
			Pattern{loadc,	1,	[](const Symbol* sym) -> bool {return sym->constVal.has_value(); }},
			Pattern{noloadl,0,	[](const Symbol* sym) -> bool {return (!sym->constVal.has_value() && sym->varType == "int"); }},
			Pattern{noloadb,0,	[](const Symbol* sym) -> bool {return (!sym->constVal.has_value() && sym->varType == "char"); }},
		};

		std::string OPToString(PatternOp op)
		{
			if (compilerFlags::g_AddComents)
			{
				switch (op) {
				case plus: return "plus";
				case min: return "min";
				case mul: return "mul";
				case div: return "div";
				default: return "";
				}
			}
			else
				return "";
		}

	};

	class PlusCondition
	{
		PlusCondition() = default;

	public:

		static bool Match(const PatternInst i1, const PatternInst i2)
		{
			// No register operand -> not allowed
			if (i1 > 2 && i2 > 2)
				return false;

			// Is there one byte operand?
			if (i1 == loadb || i2 == loadb || i1 == noloadb || i2 == noloadb)
			{
				if (i1 == i2) // Both loadb (can't and shouldn't be both noloadb because if statement above)
					return true;
				else if (i1 == noloadb && i2 == loadb || i2 == noloadb && i1 == loadb) // loadb and noloadb
					return true;

				return false; // If one is a byte both have to be
			}

			return true;
		}

		static float GetCost(const Pattern& p1, const Pattern& p2)
		{
			float opCost;
			if (p1.instr > 2 || p2.instr > 2) //Already made sure both cant be > 2 in Match()
			{
				if (p1.instr == loadc) // const, reg
					opCost = 1;
				else if (p1.instr == noloadl || p1.instr == noloadb) // mem, reg
					opCost = 1.25f;
				else if (p2.instr == noloadl || p2.instr == noloadb) // reg, mem
					opCost = 7; //Possible further improvement would be to consider a swap of operands when it gets here
				else
					assert(!"should never reach this");
			}
			else // reg, reg
				opCost = 1;
			//else
			//	assert(!"should never reach this");

			return opCost + p1.cost + p2.cost;
		}
	};
}