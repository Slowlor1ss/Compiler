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