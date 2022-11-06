#pragma once
#include "antlr4-runtime.h"
#include "CricketBaseVisitor.h"

class CodeGenVisitor final : public antlrcpp::CricketBaseVisitor
{
public:
	CodeGenVisitor() = default;
	//~CodeGenVisitor() override;

	std::any visitProg(antlrcpp::CricketParser::ProgContext* ctx) override;
	std::any visitVartype(antlrcpp::CricketParser::VartypeContext* ctx) override;
	std::any visitBeginBlock(antlrcpp::CricketParser::BeginBlockContext* ctx) override;
	std::any visitEndBlock(antlrcpp::CricketParser::EndBlockContext* ctx) override;
	std::any visitFuncDeclr(antlrcpp::CricketParser::FuncDeclrContext* ctx) override;
	std::any visitMainDeclrHeaderWithRet(antlrcpp::CricketParser::MainDeclrHeaderWithRetContext* ctx) override;
	std::any visitMainDeclrHeaderNoRet(antlrcpp::CricketParser::MainDeclrHeaderNoRetContext* ctx) override;
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
	std::any visitVarDeclr(antlrcpp::CricketParser::VarDeclrContext* ctx) override;
	std::any visitVarDeclrAndAssign(antlrcpp::CricketParser::VarDeclrAndAssignContext* ctx) override;
	std::any visitExprReturn(antlrcpp::CricketParser::ExprReturnContext* ctx) override;
	std::any visitEmptyReturn(antlrcpp::CricketParser::EmptyReturnContext* ctx) override;

	//possibly usefull for debugging and keeping track of the results
//protected:
//	virtual std::any aggregateResult(std::any /*aggregate*/, std::any nextResult) override {
//		return nextResult;
//	}

private:
	std::any visitFuncDeclrHeader(antlrcpp::CricketParser::FuncDeclrContext* ctx);
	std::any visitFuncDeclrBody(antlrcpp::CricketParser::FuncDeclrContext* ctx);

};

