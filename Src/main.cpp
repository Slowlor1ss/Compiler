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
	ErrorLogger errorLogger{};
	try
	{
		ControlFlowGraph controlFlowGraph{};

		//ANTLRInputStream input("a + (x * (y ? 0 : 1) + z);");
		//ANTLRInputStream input("b+c*d; (b+c)*d;");
		//ANTLRInputStream input("void main()\n{\n\tint a = 2;\n\tint b = 3;\n\tint c = a + b;\n}");
		//ANTLRInputStream input("int test()\n{\n    int b = 2;\n    int c = 3;\n    int d = c+d;\n    return d;\n}\n\nvoid main()\n{\n    test();\n}");
		//ANTLRInputStream input("void test()\n{\n    char b = 'l';\n    int c = 3;\n    int d = c+d;\n    return;\n}\n\nvoid main()\n{\n    test();\n}");
		ANTLRInputStream input("int func3(int x, int y, int z, int a, int b, int c, int d, int e, int h, int i, int j)\n{\n    int f = d + e + h + i + j;\n    int g = e;\n    int test = 5;\n    int test2 = 5;\n    int test3 = 5;\n\n    return b;\n}\n\nint main()\n{\n        int a = 2;\n        int b = 3;\n        int c = 3;\n        //int c = a + b;\n        int d = 5;\n        int e = 5;\n\n        func3(5,5,5,5,5,5,b,c,d,e,a);\n}");
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

		std::stringstream stream;
		controlFlowGraph.GenerateX86(stream);

		std::ofstream file("C:\\School_3rdyr\\Gradwork\\Project\\Compiler\\out.o", std::ios::out | std::ios::trunc);

		if (!file)
		{
			std::cerr << "Couldn't open file" << '\n';
			return EXIT_FAILURE;
		}

		file << stream.rdbuf();

		//std::cout << tree->toStringTree(&parser, true) << std::endl;
	}
	catch (...)
	{
		errorLogger.Signal(ERROR, "Caught exception in main " + antlrcpp::what(), 0);
	}

	return 0;
}
