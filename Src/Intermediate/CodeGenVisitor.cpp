#include "CodeGenVisitor.h"

std::any CodeGenVisitor::visitProg(antlrcpp::CricketParser::ProgContext* ctx)
{
	int numOfFunctions = ctx->funcDeclr().size();

	//go over all the function headers save their name etc make sure they aren't ambiguous
	for (int i = 0; i < numOfFunctions; i++) {
		visitFuncDeclrHeader(ctx->funcDeclr(i));
	}

	//go over all the function bodies
	for (int i = 0; i < numOfFunctions; i++) {
		visitFuncDeclrBody(ctx->funcDeclr(i));
	}

	//visit main
	visit(ctx->mainDeclr());

	return 0;
}

std::any CodeGenVisitor::visitVartype(antlrcpp::CricketParser::VartypeContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitBeginBlock(antlrcpp::CricketParser::BeginBlockContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitEndBlock(antlrcpp::CricketParser::EndBlockContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitFuncDeclr(antlrcpp::CricketParser::FuncDeclrContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitMainDeclrHeaderWithRet(antlrcpp::CricketParser::MainDeclrHeaderWithRetContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitMainDeclrHeaderNoRet(antlrcpp::CricketParser::MainDeclrHeaderNoRetContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitMainDeclr(antlrcpp::CricketParser::MainDeclrContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitParExpr(antlrcpp::CricketParser::ParExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitVarExpr(antlrcpp::CricketParser::VarExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitUnaryExpr(antlrcpp::CricketParser::UnaryExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitFuncExpr(antlrcpp::CricketParser::FuncExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitAddSubExpr(antlrcpp::CricketParser::AddSubExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitCmpEqualityLessGreaterExpr(antlrcpp::CricketParser::CmpEqualityLessGreaterExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitBitwiseExpr(antlrcpp::CricketParser::BitwiseExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitMulDivModExpr(antlrcpp::CricketParser::MulDivModExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitCmpEqualityExpr(antlrcpp::CricketParser::CmpEqualityExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitCmpLessOrGreaterExpr(antlrcpp::CricketParser::CmpLessOrGreaterExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitConstExpr(antlrcpp::CricketParser::ConstExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitAssignExpr(antlrcpp::CricketParser::AssignExprContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitPmmdEqual(antlrcpp::CricketParser::PmmdEqualContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitIfStatement(antlrcpp::CricketParser::IfStatementContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitElseStatement(antlrcpp::CricketParser::ElseStatementContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitWhileStatement(antlrcpp::CricketParser::WhileStatementContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitVarDeclr(antlrcpp::CricketParser::VarDeclrContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitVarDeclrAndAssign(antlrcpp::CricketParser::VarDeclrAndAssignContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitExprReturn(antlrcpp::CricketParser::ExprReturnContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitEmptyReturn(antlrcpp::CricketParser::EmptyReturnContext* ctx)
{
	return std::any();
}

std::any CodeGenVisitor::visitFuncDeclrHeader(antlrcpp::CricketParser::FuncDeclrContext* ctx)
{

	return 0;
}

std::any CodeGenVisitor::visitFuncDeclrBody(antlrcpp::CricketParser::FuncDeclrContext* ctx)
{
	visit(ctx->beginBlock());
	visit(ctx->body());
	visit(ctx->endBlock());

	return 0;
}