#pragma once
#include "antlr4-runtime.h"
#include "CricketBaseVisitor.h"

struct Symbol;
struct Function;
class ControlFlowGraph;
class ErrorLogger;
class Scope;
class SymbolTable;

class CodeVisitor final : public antlrcpp::CricketBaseVisitor
{
public:
	CodeVisitor(ErrorLogger& errorHandeler, ControlFlowGraph& cfg);
	//~CodeVisitor() override;

	std::any visitProg(antlrcpp::CricketParser::ProgContext* ctx) override;
	std::any visitFuncDeclr(antlrcpp::CricketParser::FuncDeclrContext* ctx) override;
	std::any visitBeginBlock(antlrcpp::CricketParser::BeginBlockContext* ctx) override;
	std::any visitEndBlock(antlrcpp::CricketParser::EndBlockContext* ctx) override;
	std::any visitBody(antlrcpp::CricketParser::BodyContext* ctx) override;
	std::any visitVarDeclr(antlrcpp::CricketParser::VarDeclrContext* ctx) override;
	std::any visitVarDeclrAndAssign(antlrcpp::CricketParser::VarDeclrAndAssignContext* ctx) override;
	std::any visitMainDeclrHeader(antlrcpp::CricketParser::MainDeclrHeaderContext* ctx) override;
	std::any visitMainDeclr(antlrcpp::CricketParser::MainDeclrContext* ctx) override;
	std::any visitParExpr(antlrcpp::CricketParser::ParExprContext* ctx) override;
	std::any visitVarExpr(antlrcpp::CricketParser::VarExprContext* ctx) override;
	std::any visitUnaryExpr(antlrcpp::CricketParser::UnaryExprContext* ctx) override;
	std::any visitFuncExpr(antlrcpp::CricketParser::FuncExprContext* ctx) override;
	std::any visitAddSubExpr(antlrcpp::CricketParser::AddSubExprContext* ctx) override;
	std::any visitCmpEqualityLessGreaterExpr(antlrcpp::CricketParser::CmpEqualityLessGreaterExprContext* ctx) override;
	std::any visitBitwiseExpr(antlrcpp::CricketParser::BitwiseExprContext* ctx) override;
	std::any visitMulDivModExpr(antlrcpp::CricketParser::MulDivModExprContext* ctx) override;
	std::any visitCmpEqualityExpr(antlrcpp::CricketParser::CmpEqualityExprContext* ctx) override;
	std::any visitCmpLessOrGreaterExpr(antlrcpp::CricketParser::CmpLessOrGreaterExprContext* ctx) override;
	std::any visitConstExpr(antlrcpp::CricketParser::ConstExprContext* ctx) override;
	std::any visitAssignExpr(antlrcpp::CricketParser::AssignExprContext* ctx) override;
	std::any visitPmmdEqual(antlrcpp::CricketParser::PmmdEqualContext* ctx) override;
	std::any visitIfStatement(antlrcpp::CricketParser::IfStatementContext* ctx) override;
	std::any visitElseStatement(antlrcpp::CricketParser::ElseStatementContext* ctx) override;
	std::any visitWhileStatement(antlrcpp::CricketParser::WhileStatementContext* ctx) override;
	std::any visitExprReturn(antlrcpp::CricketParser::ExprReturnContext* ctx) override;
	std::any visitEmptyReturn(antlrcpp::CricketParser::EmptyReturnContext* ctx) override;

	//possibly usefull for debugging and keeping track of the results
//protected:
//	virtual std::any aggregateResult(std::any /*aggregate*/, std::any nextResult) override {
//		return nextResult;
//	}

private:
	std::any visitFuncDeclrHeader(antlrcpp::CricketParser::FuncDeclrContext* ctx);

	void AddReturnDefaultInstr(antlr4::ParserRuleContext* ctx) const;
	//creates a temp symbol and adds it to the sym list of the current scope
	Symbol* CreateTempSymbol(const antlr4::ParserRuleContext* ctx, const std::string& varType, int* constPtr = nullptr);
	std::string GetFuncUniqueName(std::string funcName, const std::vector<std::string>& paramTypes);
	std::string GetFuncUniqueName(std::string funcName, const std::vector<Symbol*>& symbols);
	// Throws if one of the symbols has void as type
	void CheckUnsupportedVoidType(const size_t lineNr, const std::initializer_list<Symbol*>& symbols) const;

	int m_TempVarId{0};

	ErrorLogger& m_ErrorLogger;
	ControlFlowGraph& m_Cfg;

	SymbolTable* m_GlobalSymbolTable;
	Scope* m_GlobalScope;

	Function* m_CurrentFunction{};
};


