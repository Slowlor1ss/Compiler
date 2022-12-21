#include "CodeVisitor.h"

#include <utility>
#include "BasicBlock.h"
#include "ControlFlowGraph.h"
#include "Instruction.h"
#include "../ErrorHandeler.h"
#include "../Scope.h"
#include "../SymbolTable.h"

using namespace antlrcpp;

CodeVisitor::CodeVisitor(ErrorLogger& errorHandeler, ControlFlowGraph& cfg)
	: m_ErrorLogger(errorHandeler)
	, m_Cfg(cfg)
	, m_CurrentFunction(nullptr)
{
	m_GlobalSymbolTable = new SymbolTable();
	m_GlobalScope = m_GlobalSymbolTable->GetGlobalScope();
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


void CodeVisitor::AddReturnDefaultInstr(antlr4::ParserRuleContext* ctx) const
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
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Return, "", {"0"}, scope);
	}
	else
	{
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Return, "", {}, scope);
	}
}

Symbol* CodeVisitor::CreateTempSymbol(const antlr4::ParserRuleContext* ctx, const std::string& varType, int* constPtr)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	Symbol* sym  = scope->AddSymbol("[TempSym_" + std::to_string(m_TempVarId++) + "]", varType, ctx->getStart()->getLine(), constPtr);
	sym->isUsed = true;

	return sym;
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

	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Prologue, "", { "main" }, newSymbolTable);

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
	// Static analysis
	Scope* scope = m_GlobalSymbolTable->CurrentScope();
	scope->CheckUnusedSymbols(m_ErrorLogger);

	//TODO: if (haha get it "if") I implement if and else statements add exit true and false conditionals

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

#ifdef _DEBUG
	std::string funcName = ctx->Identifier(0)->getText();
	if (headerFunc->funcName != funcName)
	{
		m_ErrorLogger.Signal(ERROR, "Function name passed from \"visitFuncDeclrHeader()\" does not match function name ANTLR returns", ctx->getStart()->getLine());
		return EXIT_FAILURE;
	}
#endif

	m_CurrentFunction = headerFunc;

	// Returns the scope that was newly created in visitBeginBlock()
	Scope* newSymbolTable = std::any_cast<Scope*>(visit(ctx->beginBlock()));
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
	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Prologue, "", { headerFunc->funcName }, newSymbolTable);
	// https://www.reddit.com/r/learnprogramming/comments/2q8hst/why_in_x86_asm_are_parameters_given_backwards/
	for (size_t i = headerFunc->nbParameters - 1; i != size_t(-1); i--)
	{
		// Create instruction that moves function parameter from register to stack frame
		// sort of preloading the function parameters if any (gets optimized away later if unused and if optimizations are turned on)
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::ReadParam, "", { headerFunc->parameterNames[i], std::to_string(i)}, newSymbolTable);
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
	const std::string funcName = ctx->Identifier(0)->getText();

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
	if (ctx->TVoid().size() == 2 && returnType->getType() == CricketParser::TVoid || ctx->TVoid().size() == 1 && returnType->getType() != CricketParser::TVoid)
	{
		numParams = 0;
	}

	// Check errors
	if (m_GlobalScope->HasFunc(funcName))
		throw FunctionRedefenition(funcName, ctx->getStart()->getLine());

	// Add function in symbol table
	Function* func = m_GlobalScope->AddFunc(funcName, returnType->getText(), numParams, paramTypes, paramNames, ctx->getStart()->getLine());
	m_Cfg.CreateNewCurrBB(func);

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

		scope->AddSymbol(symName, symType, ctx->getStart()->getLine());
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Declaration, "", { symType, symName }, scope);
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
	scope->AddSymbol(lhsSymName, lhsSymType, ctx->getStart()->getLine());

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

	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Assign, lhsSymName, { rhsResult->varName }, scope);

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
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::NotEqual, resultTemp->varName, { sym->varName }, scope);
		break;
	case CricketParser::Minus:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Negate, resultTemp->varName, { sym->varName }, scope);
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
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Mul, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::Div:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Div, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::Mod:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Mod, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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
	//TODO: if i ever add more types this part has to change but for now  only have int and char so rhsResult will always be int
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->ADDITIVE->getType())
	{
	case CricketParser::Minus:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Minus, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::Plus:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Plus, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), {lhsSym, rhsSym});

	switch (ctx->CMP->getType())
	{
	case CricketParser::LessThan:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::LessThan, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::GreaterThan:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::GreaterThan, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->EQ->getType())
	{
	case CricketParser::Equal:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Equal, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::NotEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::NotEqual, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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
	Symbol* resultTemp = CreateTempSymbol(ctx, "int");

	// In case a void type was returned throw Unsupported_Expression (by default gets caught in visitBody) 
	CheckUnsupportedVoidType(ctx->getStart()->getLine(), { lhsSym, rhsSym });

	switch (ctx->LGEQ->getType())
	{
	case CricketParser::LessOrEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::LessOrEqual, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::GreaterOrEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::GreaterOrEqual, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::BitwiseAnd, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::BitwiseOr:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::BitwiseOr, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
		break;
	case CricketParser::BitwiseXor:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::BitwiseXor, resultTemp->varName, { lhsSym->varName, rhsSym->varName }, scope);
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

	std::string funcName = ctx->Identifier()->getText();

	// Check if function exists
	if (!m_GlobalScope->HasFunc(funcName))
		throw Unknown_FunctionCall(funcName, ctx->getStart()->getLine());

	Function* func = m_GlobalScope->GetFunc(funcName);

	// Check wrong parameter amount
	const auto numParams = ctx->primaryExpr().size();
	if (numParams != func->nbParameters) {
		throw ArgumentsAmountMismatch(funcName, ctx->getStart()->getLine());
	}

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

	// Add instructions for all the parameters
	for (size_t i = numParams - 1; i != size_t(-1); i--) 
	{
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::WriteParam, "", {params[i]->varName, std::to_string(i)}, scope);
	}

	// Create instruction for the function call
	Symbol* resultTemp = CreateTempSymbol(ctx, func->returnType);
	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Call, resultTemp->varName, { funcName, std::to_string(numParams) }, scope);
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

			resultTemp = CreateTempSymbol(ctx, "int", new int(value));

			// If optimazations are turned off add an instruction for loading the literal otherwise this is not really needed
			// can be usefull for looking at what instructions are happening
			if (!m_Cfg.GetOptimized())
				m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::WriteConst, resultTemp->varName, { std::to_string(value) }, scope);

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

			resultTemp = CreateTempSymbol(ctx, "char", new int(charLiteralValue));

			// If optimazations are turned off add an instruction for loading the literal otherwise this is not really needed
			// can be usefull for looking at what instructions are happening
			if (!m_Cfg.GetOptimized())
				m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::WriteConst, resultTemp->varName, { std::to_string(charLiteralValue) }, scope);

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

	// Add the operation to save the expression into the variable 
	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Assign, lhsName, { rhsResult->varName }, scope);
	//TODO: maybe needs a cpy?
	return scope->GetSymbol(lhsName);
}

std::any CodeVisitor::visitPmmdEqual(antlrcpp::CricketParser::PmmdEqualContext* ctx)
{
	Scope* scope = m_GlobalSymbolTable->CurrentScope();

	std::string lhsName = ctx->Identifier()->getText();
	if (!scope->HasSymbol(lhsName))
		throw Unknown_Symbol(lhsName, ctx->getStart()->getLine());

	Symbol* rhsResult = std::any_cast<Symbol*>(visit(ctx->primaryExpr()));

	switch (ctx->OPPMMD->getType())
	{
	case CricketParser::PlusEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::PlusEqual, lhsName, { rhsResult->varName }, scope);
		break;
	case CricketParser::MinusEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::MinusEqual, lhsName, { rhsResult->varName }, scope);
		break;
	case CricketParser::MulEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::MulEqual, lhsName, { rhsResult->varName }, scope);
		break;
	case CricketParser::DivEqual:
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::DivEqual, lhsName, { rhsResult->varName }, scope);
		break;
	default:
		throw Unsupported_Expression("Used unsupported operation type: " + ctx->OPPMMD->toString() + ", expected '+=', '-=', '*=', or '/='", ctx->getStart()->getLine());
	}

	return scope->GetSymbol(lhsName);
}

#pragma endregion Assign

//TODO: Implement
#pragma region Statements
std::any CodeVisitor::visitIfStatement(antlrcpp::CricketParser::IfStatementContext* ctx)
{
	return std::any();
}

std::any CodeVisitor::visitElseStatement(antlrcpp::CricketParser::ElseStatementContext* ctx)
{
	return std::any();
}

std::any CodeVisitor::visitWhileStatement(antlrcpp::CricketParser::WhileStatementContext* ctx)
{
	return std::any();
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

	m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Return, "", { rhsResult->varName }, scope);

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
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Return, "", { "0" }, scope);
	}
	else
	{
		m_Cfg.CurrentBB()->AddInstr(Instruction::Operation::Return, "", {}, scope);
	}

	return EXIT_SUCCESS;
}
