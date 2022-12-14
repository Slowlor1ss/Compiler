#include <iostream>

#include "antlr4-runtime.h"
#include "CricketLexer.h"
#include "CricketParser.h"
#include "ErrorHandeler.h"

#include "Scope.h"

#include "Intermediate/CodeVisitor.h"
#include "Intermediate/ControlFlowGraph.h"

using namespace antlrcpp;
using namespace antlr4;

int main(int argc, char* argv[])
{
	try
	{
		ErrorLogger errorLogger{};
		ControlFlowGraph controlFlowGraph{};

		//ANTLRInputStream input("a + (x * (y ? 0 : 1) + z);");
		//ANTLRInputStream input("b+c*d; (b+c)*d;");
		//ANTLRInputStream input("void main()\n{\n\tint a = 2;\n\tint b = 3;\n\tint c = a + b;\n}");
		//ANTLRInputStream input("int test()\n{\n    int b = 2;\n    int c = 3;\n    int d = c+d;\n    return d;\n}\n\nvoid main()\n{\n    test();\n}");
		ANTLRInputStream input("void test()\n{\n    char b = 'l';\n    int c = 3;\n    int d = c+d;\n    return;\n}\n\nvoid main()\n{\n    test();\n}");
		std::cout << input.toString();
		std::cout << "\n--------------------------\n";
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
			exit( EXIT_FAILURE);
		}
		if (lexer.getNumberOfSyntaxErrors() != 0)
		{
			std::cerr << "ERROR: syntax error(s) while scanning\n";
			exit(EXIT_FAILURE);
		}

		CodeVisitor codeVisitor{ errorLogger, controlFlowGraph };
		codeVisitor.visit(tree);
		//Check for errors after visiting the code
		if (errorLogger.HasError())
			exit(EXIT_FAILURE);

		//std::cout << tree->toStringTree(&parser, true) << std::endl;
	}
	catch (...)
	{
		antlrcpp::what();
	}

	return 0;
}
