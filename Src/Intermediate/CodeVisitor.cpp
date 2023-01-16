#include "CodeVisitor.h"

#include <utility>
#include "BasicBlock.h"
#include "BasicBlock.h"
#include "ControlFlowGraph.h"
#include "Instruction.h"
#include "../ErrorHandeler.h"
#include "../Scope.h"
#include "../SymbolTable.h"
#include "../CompilerFlags.hpp"

using namespace antlrcpp;

CodeVisitor::CodeVisitor(ErrorLogger& errorHandeler, ControlFlowGraph& cfg)
	: m_ErrorLogger(errorHandeler)
	, m_Cfg(cfg)
	, m_CurrentFunction(nullptr)
{
	m_GlobalSymbolTable = new SymbolTable();
	m_GlobalScope = m_GlobalSymbolTable->GetGlobalScope();
	m_Cfg.SetSymbolTable(m_GlobalSymbolTable);
}

std::any CodeVisitor::visitProg(antlrcpp::CricketParser::ProgContext* ctx)
{
	const size_t numOfFunctions = ctx->funcDeclr().size();

	//go over all the function
	for (size_t i = 0; i < numOfFunctions; ++i) {
		visitFuncDeclr(ctx->funcDeclr(i));
	}

	//TODO: maybe visit main first, do we make sure there's no other function that's called main in global scope
	//TODO: probably shouldn't do the above as otherwise functions used inside main will be unknown HOWEVER maybe i should first go over the functions above main then main then functions under main
	//visit main
	visit(ctx->mainDeclr());

	return EXIT_SUCCESS;
}


void CodeVisitor::AddReturnDefaultInstr(antlr4::ParserRuleContext* ctx)
{
	// Set flag
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	scope->SetReturned(true);

	// Check warnings
	//antlrcpp::CricketParser::TVoid
	const std::string functionName = m_CurrentFunction->funcName;
	if (m_CurrentFunction->returnType != "void" && functionName != "main")
	{
		const std::string message = "No \"return\" found in non-void function \"" + functionName + "\" -> Defaulted to returning 0";
		m_ErrorLogger.Signal(WARNING, message, ctx->getStart()->getLine());
	}

	// Add return instruction
	if(functionName == "main" || m_CurrentFunction->returnType != "void")
	{
		auto* tempSym = CreateTempSymbol(ctx, m_CurrentFunction->returnType, 0);
		m_Cfg.CurrentBB()->AddInstr(new Operation::Return(tempSym, scope));
	}
	else
	{
		m_Cfg.CurrentBB()->AddInstr(new Operation::Return(nullptr, scope));
	}
}

//TODO: add optional parameter for better name and combine tempSymX and name
Symbol* CodeVisitor::CreateTempSymbol(const antlr4::ParserRuleContext* ctx, const std::string& varType, std::optional<int> constVal)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	Symbol* sym;
	if (compilerFlags::g_RemoveTempVars)
		sym = scope->AddTempSymbol("[Temp_" + std::to_string(m_TempVarId++) + "]", varType, ctx->getStart()->getLine(), constVal);
	else
		sym = scope->AddSymbol("[Temp_" + std::to_string(m_TempVarId++) + "]", varType, ctx->getStart()->getLine(), constVal);
	sym->isUsed = true;

	return sym;
}

std::string CodeVisitor::GetFuncUniqueName(std::string funcName, const std::vector<std::string>& paramTypes)
{
	//Generate unique name for every function buy adding the first letter of the param types
	if (!paramTypes.empty())
	{
		funcName += +"_";
		for (auto& pt : paramTypes)
			funcName += pt[0];
	}

	return funcName;
}
std::string CodeVisitor::GetFuncUniqueName(std::string funcName, const std::vector<Symbol*>& symbols)
{
	//Generate unique name for every function buy adding the first letter of the param types
	if (!symbols.empty())
	{
		funcName += +"_";
		for (auto& sym : symbols)
			funcName += sym->varType[0];
	}

	return funcName;
}


void CodeVisitor::CheckUnsupportedVoidType(const size_t lineNr, const std::initializer_list<Symbol*>& symbols) const
{
	for (const auto& symbol : symbols)
	{
		if (symbol->varType == "void")
		{
			const std::string message = std::format("{} void type operation not supported", symbol->varName);
			throw Unsupported_Expression(message, lineNr);
		}
	}
}


#pragma region MainDeclr

// Maybe something to keep in mind:
// https://stackoverflow.com/questions/1724594/x86-assembly-whats-the-main-prologue-and-epilogue
std::any CodeVisitor::visitMainDeclrHeader(antlrcpp::CricketParser::MainDeclrHeaderContext* ctx)
{
	Function* func = m_GlobalScope->AddFunc("main", ctx->FTYPE->getText(), 0, {}, {}, ctx->getStart()->getLine());
	m_Cfg.CreateNewCurrBB(func);
	return func;
}

std::any CodeVisitor::visitMainDeclr(antlrcpp::CricketParser::MainDeclrContext* ctx)
{
	Function* headerFunc;
	try
	{
		headerFunc = std::any_cast<Function*>(visit(ctx->mainDeclrHeader()));
	}
	catch (Redefenition_Error& e)
	{
		m_ErrorLogger.Signal(ERROR, e.what(), e.lineNr());
		return EXIT_FAILURE;
	}

	m_CurrentFunction = headerFunc;

	// Returns the scope that was newly created in visitBeginBlock()
	Scope* newSymbolTable = std::any_cast<Scope*>(visit(ctx->beginBlock()));

	m_Cfg.CurrentBB()->AddInstr(new Operation::Prologue(newSymbolTable));

	visit(ctx->body());

	if (!newSymbolTable->HasReturned()) AddReturnDefaultInstr(ctx);

	visit(ctx->endBlock());

	return EXIT_SUCCESS;
}

#pragma endregion MainDeclr


std::any CodeVisitor::visitBeginBlock(antlrcpp::CricketParser::BeginBlockContext* ctx)
{
	// Add scope to the scope stack with current scope as parent scope
	return m_GlobalSymbolTable->PushScope(m_GlobalSymbolTable->CurrentScope());
}

std::any CodeVisitor::visitEndBlock(antlrcpp::CricketParser::EndBlockContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	scope->CheckUnusedSymbolsAndResetIsUsed(m_ErrorLogger);

	BasicBlock* currBB = m_Cfg.CurrentBB();

#ifdef _DEBUG
	if (currBB->GetExitFalse()) 
	{
		assert(!"Should never reach this"); //!string == false
		//currBB->AddInstr(new Operation::ConditionalJump(conditionResult, currBB->GetExitTrue()->GetLabel(), currBB->GetExitFalse()->GetLabel(), scope));
	}
#endif
	if (currBB->GetExitTrue())
		currBB->AddInstr(new Operation::UnconditionalJump(currBB->GetExitTrue()->GetLabel(), scope));

	// Remove scope from the scope stack
	m_GlobalSymbolTable->PopScope();

	return 0;
}


std::any CodeVisitor::visitFuncDeclr(antlrcpp::CricketParser::FuncDeclrContext* ctx)
{
	Function* headerFunc;
	try
	{
		headerFunc = std::any_cast<Function*>(visitFuncDeclrHeader(ctx));
	}
	catch (Redefenition_Error& e)
	{
		m_ErrorLogger.Signal(ERROR, e.what(), e.lineNr());
		return EXIT_FAILURE;
	}

	m_CurrentFunction = headerFunc;

	// Returns the scope that was newly created in visitBeginBlock()
	Scope* newSymbolTable = std::any_cast<Scope*>(visit(ctx->beginBlock()));
	headerFunc->scope = newSymbolTable;
	// Scope* newSymbolTable = m_GlobalSymbolTable->CurrentScope();

	int paramStackOffset = 16;	// The size of the return address RAX(64-bit) or EAX(32-bit) stored on the stack when calling the function (in our case RAX)
	// RAX is only 8 bytes but the RBP already takes the first 8 bytes so we start from 16 -> |-8| localVar |0| RBP |8| RAX |16| param |24|
	// (used for when there are more parameters than register to hold them, then these will be pushed on the stack but behind the return value)

	// Add func-params in the function it's scoped symbol table
	for (size_t i{0}; i < headerFunc->nbParameters; ++i)
	{
		int sp = newSymbolTable->GetStackPointer();
		auto sym = newSymbolTable->AddSymbol(headerFunc->parameterNames[i], headerFunc->parameterTypes[i], ctx->getStart()->getLine());
		if (i+1 > Instruction::GetAmountOfRegisters()) // i+1 bc its starts from 0 and amntRegisters doesn't
		{
			sym->memoryOffset = paramStackOffset; // This will be the new mem offset
			// Make room for next param that gets pushed on the stack (note: this is only used when there are more than 6 parameters)
			paramStackOffset += 8;

			newSymbolTable->SetStackPointer(sp); // Set the stack pointer back as if this param wasn't added
		}
	}

	// Create prologue instruction
	m_Cfg.CurrentBB()->AddInstr(new Operation::Prologue(newSymbolTable));
	// https://www.reddit.com/r/learnprogramming/comments/2q8hst/why_in_x86_asm_are_parameters_given_backwards/
	for (size_t i = headerFunc->nbParameters - 1; i != size_t(-1); i--)
	{
		// Create instruction that moves function parameter from register to stack frame
		// sort of preloading the function parameters if any (gets optimized away later if unused and if optimizations are turned on)
		m_Cfg.CurrentBB()->AddInstr(new Operation::ReadParam(headerFunc->parameterNames[i], i, newSymbolTable));
	}

	// Create body instructions
	visit(ctx->body());

	// If not returned yet; return
	if (!newSymbolTable->HasReturned())
	{
		AddReturnDefaultInstr(ctx);
	}

	// Visit end (discard symbol table)
	visit(ctx->endBlock());

	return EXIT_SUCCESS;
}

std::any CodeVisitor::visitFuncDeclrHeader(antlrcpp::CricketParser::FuncDeclrContext* ctx)
{
	std::string funcName = ctx->Identifier(0)->getText();

	// Get the return type
	auto returnType = ctx->FTYPE;

	// Get all parameter names and their types
	std::vector<std::string> paramTypes = {};
	std::vector<std::string> paramNames = {};

	size_t numParams = ctx->Identifier().size() - 1;
	for (size_t i = 0; i < numParams; ++i)
	{
		std::string paramName = ctx->Identifier(1 + i)->getText();
		std::string paramType = ctx->vartype(i)->getText();
		paramTypes.push_back(paramType);
		paramNames.push_back(paramName);
	}

	//Use the unique func name to allow function overloading
	funcName = GetFuncUniqueName(funcName, paramTypes);

	// Check errors
	if (m_GlobalScope->HasFunc(funcName))
		throw FunctionRedefenition(funcName, ctx->getStart()->getLine());

	// Add function in symbol table
	//------TODO: combine these 2-------
	Function* func = m_GlobalScope->AddFunc(funcName, returnType->getText(), numParams, paramTypes, paramNames, ctx->getStart()->getLine());
	m_Cfg.CreateNewCurrBB(func);
	//----------------------------------

	return func;
}

// Purely here to log errors while checking code
std::any CodeVisitor::visitBody(antlrcpp::CricketParser::BodyContext* ctx)
{
	std::any result = defaultResult();
	size_t n = ctx->children.size();

	for (size_t i = 0; i < n; i++) 
	{
		if (!shouldVisitNextChild(ctx, result))
			break;

		try
		{
			std::any childResult = ctx->children[i]->accept(this);
			result = aggregateResult(std::move(result), std::move(childResult));
		}
		catch (Cricket_Error& e)
		{
			// When catching an unsupported expression log the m_Error and continue checking the rest of the program for more errors and warnings
			m_ErrorLogger.Signal(ERROR, e.what(), e.lineNr());
			continue;
		}

	}

	return result;
}


#pragma region VarDeclr

//ex. int a, b, c;
std::any CodeVisitor::visitVarDeclr(antlrcpp::CricketParser::VarDeclrContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	// Only one type
	std::string symType = ctx->vartype()->getText();
	// But can have multiple names in cases like: "int a, b, c;"
	const size_t numVariable = ctx->Identifier().size();
	for (size_t i = 0; i < numVariable; ++i) 
	{
		std::string symName = ctx->Identifier(i)->getText();

		// Check redefinition
		if (scope->CheckSymbolRedefinition(symName, ctx->getStart()->getLine(), m_ErrorLogger))
			return EXIT_FAILURE;

		auto* sym = scope->AddSymbol(symName, symType, ctx->getStart()->getLine());
		m_Cfg.CurrentBB()->AddInstr(new Operation::Declaration(sym, scope));
	}
	return EXIT_SUCCESS;
}

//ex. int a = 5; (not to confuse with a = 5; which would just be assign expr (no declaration))
std::any CodeVisitor::visitVarDeclrAndAssign(antlrcpp::CricketParser::VarDeclrAndAssignContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	std::string lhsSymType = ctx->vartype()->getText();
	std::string lhsSymName = ctx->Identifier()->getText();

	// Check redefinition
	if (scope->CheckSymbolRedefinition(lhsSymName, ctx->getStart()->getLine(), m_ErrorLogger))
		return EXIT_FAILURE;

	// Declare symbol
	auto* lhsSym = scope->AddSymbol(lhsSymName, lhsSymType, ctx->getStart()->getLine());

	// Get curr sp
	int currStackPointer = scope->GetStackPointer();

	Symbol* rhsResult = std::any_cast<Symbol*>(visit(ctx->expr()));

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody)
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), {rhsResult});
	//if (rhsResult->symType == "void") 
	//{
	//	const std::string message = std::format("{} void type assignment not supported", lhsName);
	//	throw Unsupported_Expression(message, ctx->getStart()->getLine());
	//}

	// Set the sp back to previous value
	scope->SetStackPointer(currStackPointer);

	m_Cfg.CurrentBB()->AddInstr(new Operation::Assign(lhsSym, rhsResult, scope));

	return EXIT_SUCCESS;
}

#pragma endregion VarDeclr

#pragma region PrimaryExpr

std::any CodeVisitor::visitParExpr(antlrcpp::CricketParser::ParExprContext* ctx)
{
	return std::any_cast<Symbol*>(visit(ctx->expr()));
}

std::any CodeVisitor::visitUnaryExpr(antlrcpp::CricketParser::UnaryExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* sym = std::any_cast<Symbol*>(visit(ctx->primaryExpr()));
	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { sym });

	// Create a temp to hold the rhsResult of the unary expr
	Symbol* resultTemp = CreateTempSymbol(ctx, sym->varType);

	switch (ctx->UNARY->getType())
	{
	case CricketParser::ExclamationMark:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Not(resultTemp, sym, scope));
		break;
	case CricketParser::Minus:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Negate(resultTemp, sym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported unary type: " + ctx->UNARY->toString(), ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitMulDivModExpr(antlrcpp::CricketParser::MulDivModExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->MULTIPLICATIVE->getType())
	{
	case CricketParser::Mul:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Mul(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::Div:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Div(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::Mod:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Mod(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->MULTIPLICATIVE->toString() + ", expected '*', '/', or '%'", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitAddSubExpr(antlrcpp::CricketParser::AddSubExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int or char and char fits in int
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->ADDITIVE->getType())
	{
	case CricketParser::Minus:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Minus(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::Plus:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Plus(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->ADDITIVE->toString() + ", expected '-', or '+'", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitCmpLessOrGreaterExpr(antlrcpp::CricketParser::CmpLessOrGreaterExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "char");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), {lhsSym, rhsSym});

	switch (ctx->CMP->getType())
	{
	case CricketParser::LessThan:
		m_Cfg.CurrentBB()->AddInstr(new Operation::LessThan(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::GreaterThan:
		m_Cfg.CurrentBB()->AddInstr(new Operation::GreaterThan(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->CMP->toString() + ", expected '<', or '>'", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitCmpEqualityExpr(antlrcpp::CricketParser::CmpEqualityExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "char");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->EQ->getType())
	{
	case CricketParser::Equal:
		m_Cfg.CurrentBB()->AddInstr(new Operation::Equal(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::NotEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::NotEqual(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->EQ->toString() + ", expected '==', or '!='", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitCmpEqualityLessGreaterExpr(antlrcpp::CricketParser::CmpEqualityLessGreaterExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "char");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->LGEQ->getType())
	{
	case CricketParser::LessOrEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::LessOrEqual(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::GreaterOrEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::GreaterOrEqual(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->LGEQ->toString() + ", expected '<=', or '>='", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitBitwiseExpr(antlrcpp::CricketParser::BitwiseExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* lhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(0)));
	Symbol* rhsSym = std::any_cast<Symbol*>(visit(ctx->primaryExpr(1)));
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->BITWISE->getType())
	{
	case CricketParser::BitwiseAnd:
		m_Cfg.CurrentBB()->AddInstr(new Operation::BitwiseAnd(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::BitwiseOr:
		m_Cfg.CurrentBB()->AddInstr(new Operation::BitwiseOr(resultTemp, lhsSym, rhsSym, scope));
		break;
	case CricketParser::BitwiseXor:
		m_Cfg.CurrentBB()->AddInstr(new Operation::BitwiseXor(resultTemp, lhsSym, rhsSym, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->BITWISE->toString() + ", expected '&', '|', or '^'", ctx->getStart()->getLine());
	}

	return resultTemp;
}
// Function call
std::any CodeVisitor::visitFuncExpr(antlrcpp::CricketParser::FuncExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	const auto numParams = ctx->primaryExpr().size();

	// Get current stack pointer
	int currStackPointer = scope->GetStackPointer();
	// Get all parameters
	std::vector<Symbol*> params;
	for (size_t i = 0; i < numParams; i++)
	{
		params.emplace_back(std::any_cast<Symbol*>(visit(ctx->primaryExpr(i))));
	}
	// Reset the sp after visiting parameter expressions
	scope->SetStackPointer(currStackPointer);


	std::string funcName = ctx->Identifier()->getText();
	//Use the unique func name to allow function overloading
	funcName = GetFuncUniqueName(funcName, params);

	// Check if function exists
	if (!m_GlobalScope->HasFunc(funcName))
		throw Unknown_FunctionCall(funcName, ctx->getStart()->getLine());

	Function* func = m_GlobalScope->GetFunc(funcName);

	// Check wrong parameter amount
	if (numParams != func->nbParameters) {
		throw ArgumentsAmountMismatch(funcName, ctx->getStart()->getLine());
	}


	// Add instructions for all the parameters
	for (size_t i = numParams - 1; i != size_t(-1); i--) 
	{
		m_Cfg.CurrentBB()->AddInstr(new Operation::WriteParam(params[i], i, func, scope));
	}


	// Create instruction for the function call
	// TODO: test returning nullprt might cause bad_any_cast
	Symbol* resultTemp = func->returnType != "void" ? CreateTempSymbol(ctx, func->returnType) : nullptr;
	m_Cfg.CurrentBB()->AddInstr(new Operation::Call(resultTemp, funcName, numParams, scope));
	func->isCalled = true;
	m_CurrentFunction->hasFuncCall = true;

	return resultTemp;
}
// Literals
std::any CodeVisitor::visitConstExpr(antlrcpp::CricketParser::ConstExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	Symbol* resultTemp;

	switch (ctx->CONST->getType())
	{
	case CricketParser::Number:
		{
			//For now only support integer types
			auto value = std::stoll(ctx->getText());
			if (value > INT_MAX || value < INT_MIN)
			{
				value = static_cast<int>(value);
				//Send a warning to inform the user
				std::string message = "signed integral constant overflow '" + ctx->getText() + "' to '" + std::to_string(value) + "'";
				m_ErrorLogger.Signal(WARNING, message, ctx->getStart()->getLine());
			}

			resultTemp = CreateTempSymbol(ctx, "int", value);

			// If optimazations are turned off add an instruction for loading the literal otherwise this is not really needed
			// can be usefull for looking at what instructions are happening
			if (!compilerFlags::g_OptimizeConstPropagation)
				m_Cfg.CurrentBB()->AddInstr(new Operation::WriteConst(resultTemp, std::to_string(value), scope));

			break;
		}
	case CricketParser::CharLiteral:
		{
			//// Remove the surrounding ' we also only need to get one char as its a char literal
			////auto str = ctx->getText();
			////str.erase(std::ranges::remove(str, '\'').begin(), str.end());
			////auto charLiteral = str[0];
			
			// Do [1] to skip the surrounding '
			const int charLiteralValue = ctx->getText()[1]; //TODO: test a char like: char c = '\''

			resultTemp = CreateTempSymbol(ctx, "char", charLiteralValue);

			// If optimazations are turned off add an instruction for loading the literal otherwise this is not really needed
			// can be usefull for looking at what instructions are happening
			if (!compilerFlags::g_OptimizeConstPropagation)
				m_Cfg.CurrentBB()->AddInstr(new Operation::WriteConst(resultTemp, std::to_string(charLiteralValue), scope));

			break;
		}
	default:
		throw Unsupported_Expression("Used unsupported literal type: " + ctx->CONST->toString() + ", expected 'int', or 'char'", ctx->getStart()->getLine());
	}

	return resultTemp;
}

std::any CodeVisitor::visitVarExpr(antlrcpp::CricketParser::VarExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	const std::string idName = ctx->Identifier()->getText();
	// Check if symbol exists
	if (!scope->HasSymbol(idName))
		throw Unknown_Symbol(idName, ctx->getStart()->getLine());

	// Set it to used for compiler optimizations later
	auto sym = scope->GetSymbol(idName);
	sym->isUsed = true;

	return sym;
}

#pragma endregion PrimaryExpr

#pragma region Assign

std::any CodeVisitor::visitAssignExpr(antlrcpp::CricketParser::AssignExprContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	std::string lhsName = ctx->Identifier()->getText();

	if (!scope->HasSymbol(lhsName))
		throw Unknown_Symbol(lhsName, ctx->getStart()->getLine());

	// Get curr sp
	int currStackPointer = scope->GetStackPointer();

	// Get the value we want to assign
	Symbol* rhsResult = std::any_cast<Symbol*>(visit(ctx->expr()));

	// Set the sp back to previous value
	scope->SetStackPointer(currStackPointer);

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody)
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { rhsResult });

	auto* lhsSym = scope->GetSymbol(lhsName);
	// Add the operation to save the expression into the variable 
	m_Cfg.CurrentBB()->AddInstr(new Operation::Assign(lhsSym, rhsResult, scope));
	//TODO!: test case like a = b = c;
	return lhsSym;
}

std::any CodeVisitor::visitPmmdEqual(antlrcpp::CricketParser::PmmdEqualContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	std::string lhsName = ctx->Identifier()->getText();
	if (!scope->HasSymbol(lhsName))
		throw Unknown_Symbol(lhsName, ctx->getStart()->getLine());

	Symbol* lhsSym = scope->GetSymbol(lhsName);
	Symbol* rhsResult = std::any_cast<Symbol*>(visit(ctx->primaryExpr()));

	switch (ctx->OPPMMD->getType())
	{
	case CricketParser::PlusEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::PlusEqual(lhsSym, rhsResult, scope));
		break;
	case CricketParser::MinusEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::MinusEqual(lhsSym, rhsResult, scope));
		break;
	case CricketParser::MulEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::MulEqual(lhsSym, rhsResult, scope));
		break;
	case CricketParser::DivEqual:
		m_Cfg.CurrentBB()->AddInstr(new Operation::DivEqual(lhsSym, rhsResult, scope));
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->OPPMMD->toString() + ", expected '+=', '-=', '*=', or '/='", ctx->getStart()->getLine());
	}

	return scope->GetSymbol(lhsName);
}

#pragma endregion Assign

#pragma region Statements
std::any CodeVisitor::visitIfStatement(antlrcpp::CricketParser::IfStatementContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	Symbol* conditionResult = std::any_cast<Symbol*>(visit(ctx->expr(0)));

	BasicBlock* origExitTrue = m_Cfg.CurrentBB()->GetExitTrue();
	BasicBlock* origExitFalse = m_Cfg.CurrentBB()->GetExitFalse();

	BasicBlock* conditionBB = m_Cfg.CurrentBB();
	BasicBlock* bodyBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);
	// We dont create end if here so that it will always be created after else bb as this has an effect on the order of the generated code
	// and while it will still work if its created befor the else its less optimized as propagate const wont know what happens in the else
	BasicBlock* endIfBB;// = m_Cfg.CreateNewStatementBB(m_CurrentFunction);

	// If its true go inside the body
	conditionBB->SetExitTrue(bodyBB);

	// If there's also an else statement
	if (ctx->elseStatement()) {

		BasicBlock* elseBodyBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);

		// If False go inside the false body
		conditionBB->SetExitFalse(elseBodyBB);

		endIfBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);
		elseBodyBB->SetExitTrue(endIfBB);

		conditionBB->AddInstr(new Operation::ConditionalJump(conditionResult, conditionBB->GetExitTrue()->GetLabel(), conditionBB->GetExitFalse()->GetLabel(), scope));

		// Visit else body
		m_Cfg.SetCurrentBB(elseBodyBB);
		visit(ctx->elseStatement());
	}
	else {
		endIfBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);
		// If false just skip the true body and go to the end of the if
		conditionBB->SetExitFalse(endIfBB);

		conditionBB->AddInstr(new Operation::ConditionalJump(conditionResult, conditionBB->GetExitTrue()->GetLabel(), conditionBB->GetExitFalse()->GetLabel(), scope));
	}

	// Set its exit pointers to the ones of the parent BB
	endIfBB->SetExitTrue(origExitTrue);
	endIfBB->SetExitFalse(origExitFalse);

	bodyBB->SetExitTrue(endIfBB);

	m_Cfg.SetCurrentBB(bodyBB);
	// If statement with body
	if (ctx->body()) {
		visit(ctx->beginBlock());
		visit(ctx->body());
		visit(ctx->endBlock());
		// Already happens in endBlock
		//bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}
	// If statement without {}
	else if (ctx->expr(1))
	{
		visit(ctx->expr(1));
		// Write instruction to jump back to the following block
		bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}
	// If statement directly followed by a flow control statement like return, break, or continue
	else if (ctx->flowControl()) //TODO: test properly
	{
		visit(ctx->flowControl());
		// Write instruction to jump back to the following block
		bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}

	// Set the next current BB
	m_Cfg.SetCurrentBB(endIfBB);

	return EXIT_SUCCESS;
}

std::any CodeVisitor::visitElseStatement(antlrcpp::CricketParser::ElseStatementContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	BasicBlock* elseBodyBB = m_Cfg.CurrentBB();

	// Else statement with body
	if (ctx->body()) {
		visit(ctx->beginBlock());
		visit(ctx->body());
		visit(ctx->endBlock());
	}
	// Else statement without {}
	else if (ctx->expr())
	{
		visit(ctx->expr());
		elseBodyBB->AddInstr(new Operation::UnconditionalJump(elseBodyBB->GetExitTrue()->GetLabel(), scope));
	}
	// Else statement directly followed by a flow control statement like return, break, or continue
	else if (ctx->flowControl())
	{
		visit(ctx->flowControl());
		elseBodyBB->AddInstr(new Operation::UnconditionalJump(elseBodyBB->GetExitTrue()->GetLabel(), scope));
	}

	return EXIT_SUCCESS;
}

std::any CodeVisitor::visitWhileStatement(antlrcpp::CricketParser::WhileStatementContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	BasicBlock* beforeWhileBB = m_Cfg.CurrentBB();

	BasicBlock* conditionBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction); // We create a separate bb for the condition -> while(...)
	BasicBlock* bodyBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);

	m_Cfg.SetCurrentBB(conditionBB);
	Symbol* conditionResult = std::any_cast<Symbol*>(visit(ctx->expr(0)));

	BasicBlock* afterWhileBB = m_Cfg.CreateNewStatementBB(m_CurrentFunction);

	// Set its exit pointers to the ones of the parent BB
	afterWhileBB->SetExitTrue(beforeWhileBB->GetExitTrue());
	afterWhileBB->SetExitFalse(beforeWhileBB->GetExitFalse());

	beforeWhileBB->SetExitTrue(conditionBB);

	conditionBB->SetExitTrue(bodyBB);
	conditionBB->SetExitFalse(afterWhileBB);

	bodyBB->SetExitTrue(conditionBB);


	m_Cfg.SetCurrentBB(bodyBB);
	if (ctx->body()) {
		visit(ctx->beginBlock());
		visit(ctx->body());
		visit(ctx->endBlock());
		// Already happens in endBlock
		//bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}
	else if (ctx->expr(1))
	{
		visit(ctx->expr(1));
		bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}
	else if (ctx->flowControl())
	{
		visit(ctx->flowControl());
		bodyBB->AddInstr(new Operation::UnconditionalJump(bodyBB->GetExitTrue()->GetLabel(), scope));
	}
	beforeWhileBB->AddInstr(new Operation::UnconditionalJump(beforeWhileBB->GetExitTrue()->GetLabel(), scope));
	conditionBB->AddInstr(new Operation::ConditionalJump(conditionResult, conditionBB->GetExitTrue()->GetLabel(), conditionBB->GetExitFalse()->GetLabel(), scope));
	// NOTE: actually depends on in what order the basic blacks have been added as these are instructions added to different basic blocks
	// NOTO: code below is not correct anymore maybe TODO: remove
	// Example of how it will look in assembly
	//		(...)
	//		jmp.L2					#jump to the comparision
	//	.L3:						#body
	//		addl    $1, -4(% rbp)	#do whatever in the body
	//	.L2:						#comparison
	//		cmpl    $0, -4(% rbp) 
	//		jne.L3					#if its true jump to the body else just continue
	//		(...)

	m_Cfg.SetCurrentBB(afterWhileBB);

	return EXIT_SUCCESS;
}
#pragma endregion Statements


std::any CodeVisitor::visitExprReturn(antlrcpp::CricketParser::ExprReturnContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	scope->SetReturned(true);

	// Get curr sp
	int currStackPointer = scope->GetStackPointer();

	Symbol* rhsResult = std::any_cast<Symbol*>(visit(ctx->expr()));
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { rhsResult });

	// Set the sp back to previous value
	scope->SetStackPointer(currStackPointer);

	const std::string functionName = m_CurrentFunction->funcName;
	if (m_CurrentFunction->returnType != rhsResult->varType)
	{
		const std::string message = "Return type of function \"" + functionName + "\" does not match type of returned value";
		m_ErrorLogger.Signal(WARNING, message, ctx->getStart()->getLine()); //TODO: maybe make this an error
	}

	m_Cfg.CurrentBB()->AddInstr(new Operation::Return(rhsResult, scope));

	return EXIT_SUCCESS;
}

std::any CodeVisitor::visitEmptyReturn(antlrcpp::CricketParser::EmptyReturnContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	scope->SetReturned(true);

	const std::string functionName = m_CurrentFunction->funcName;
	if (m_CurrentFunction->returnType != "void")
	{
		const std::string message = "Empty return in non-void function \"" + functionName + "\" -> Defaulted to returning 0";
		m_ErrorLogger.Signal(WARNING, message, ctx->getStart()->getLine());
	}

	// Add return instruction
	if (functionName == "main" || m_CurrentFunction->returnType != "void")
	{
		auto* tempSym = CreateTempSymbol(ctx, m_CurrentFunction->returnType, 0);
		m_Cfg.CurrentBB()->AddInstr(new Operation::Return(tempSym, scope));
	}
	else
	{
		m_Cfg.CurrentBB()->AddInstr(new Operation::Return(nullptr, scope));
	}

	return EXIT_SUCCESS;
}
