#include <iostream>

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TLexer.h"
#include "TParser.h"

using namespace antlrcpptest;
using namespace antlr4;

int main(int , const char **)
{
	//ANTLRInputStream input(u8"🍴 = 🍐 + \"😎\";(((x * π))) * µ + ∰; a + (x * (y ? 0 : 1) + z);");
	//ANTLRInputStream input("a + (x * (y ? 0 : 1) + z);");
	ANTLRInputStream input("b+c*d; (b+c)*d;");
	TLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	
	//tokens.fill();
	//for (auto token : tokens.getTokens()) {
	//  std::cout << token->toString() << std::endl;
	//}
	
	//TParser::AnyContext* t = parser.any();
	TParser parser(&tokens);
	tree::ParseTree *tree = parser.main();

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

	std::cout << tree->toStringTree(&parser, true) << std::endl;
	
	return 0;
}