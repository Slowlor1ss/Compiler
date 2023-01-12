#include "OptimumTiling.h"
#include "../Scope.h"

using namespace InstrSelection;

OptimumInstructionTiling::OptimumInstructionTiling(Symbol* lhs, Symbol* rhs, bool lhsIsConst,
	const std::optional<int> constval, InstrSelection::PatternOp op) : m_Op(op)
{
	//TODO: test is this also works with const propagation off
	if (lhsIsConst)
		lhs->constVal = constval;
	else
	{
		rhs->constVal = constval;
		std::swap(lhs, rhs); // If doing an operation with a const lhs has to be the const
	}

	PatternInst i1;
	PatternInst i2;
	float cost{ INFINITY };
	for (size_t i{0}; i < m_Patterns.size(); ++i)
	{
		if (!m_Patterns[i].Match(lhs))
			continue;

		for (size_t j{0}; j < m_Patterns.size(); ++j)
		{
			if (m_Patterns[j].instr == loadc || !m_Patterns[j].Match(rhs)) // rhs can never be const
				continue;

			float costI;
			switch (m_Op)
			{
			case InstrSelection::plus:
				if(!PlusCondition::Match(m_Patterns[i].instr, m_Patterns[j].instr))
					continue;
				costI = PlusCondition::GetCost(m_Patterns[i], m_Patterns[j]);
				if (costI < cost)
				{
					i1 = m_Patterns[i].instr;
					i2 = m_Patterns[j].instr;
					cost = costI;
				}
				break;
			}


		}
	}

	//set i1 and i2 as member variables call loadreg in generate asm
	symbols = { lhs, rhs };
	inst = { i1, i2 };
}
