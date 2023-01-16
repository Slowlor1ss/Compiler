//class CompilerFlags
//{
//public:
//	CompilerFlags(const bool cprop, const bool dce, const bool cbranch, const bool regAlloc, const bool verboseAsm)
//		: _ConstPropagationAssignment(cprop)
//	{
//	}
//
//	static const bool _ConstPropagationAssignment{ false };
//	static const bool _OptimizeConstPropagation{ true };
//	static const bool _RemoveDeadcode{ true };
//	static const bool _RemoveConstConditionals{ true };
//	static const bool _OptimizeASM{ true };
//	static const bool _RemoveTempVars{ true && _OptimizeASM };
//	static const bool _AddComents{ true };
//};
#pragma once

namespace compilerFlags
{
	inline bool g_OptimizeConstPropagation{ false };
	inline bool g_RemoveDeadcode{ false };
	inline bool g_RemoveConstConditionals{ g_RemoveDeadcode };
	inline bool g_PeepholeOptimization{ false };
	inline bool g_RemoveTempVars{ false && g_PeepholeOptimization };
	inline bool g_AddComents{ false };
}