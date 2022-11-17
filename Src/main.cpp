#include <iostream>

#include "antlr4-runtime.h"
#include "CricketLexer.h"
#include "CricketParser.h"

#include "Scope.h"
#include "Intermediate/CodeGenVisitor.h"

using namespace antlrcpp;
using namespace antlr4;

int main(int , const char **)
{
	//ANTLRInputStream input(u8"🍴 = 🍐 + \"😎\";(((x * π))) * µ + ∰; a + (x * (y ? 0 : 1) + z);");
	//ANTLRInputStream input("a + (x * (y ? 0 : 1) + z);");
	ANTLRInputStream input("b+c*d; (b+c)*d;");
	CricketLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	
	//tokens.fill();
	//for (auto token : tokens.getTokens()) {
	//  std::cout << token->toString() << std::endl;
	//}
	
	//CParser::AnyContext* t = parser.any();
	CricketParser parser(&tokens);
	tree::ParseTree *tree = parser.start();

	//CricketParser::UnaryExprContext::UNARY.match(CricketParser::EOF);
	//if(CricketParser::UnaryExprContext::UNARY->getType() == CricketParser::Number){}

	if (parser.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while parsing\n";
		std::cout.flush();
		exit( EXIT_FAILURE);
	}
	if (lexer.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while scanning\n";
		std::cout.flush();
		exit(EXIT_FAILURE);
	}

	CodeGenVisitor cgVisitor{};
	cgVisitor.visit(tree);

	std::cout << tree->toStringTree(&parser, true) << std::endl;
	
	return 0;
}