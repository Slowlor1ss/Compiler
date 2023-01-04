#include <iostream>
#include <filesystem>

#include "antlr4-runtime.h"
#include "CricketLexer.h"
#include "CricketParser.h"

#include "Scope.h"
#include "ErrorHandeler.h"
#include "Intermediate/CodeVisitor.h"
#include "Intermediate/ControlFlowGraph.h"

using namespace antlrcpp;
using namespace antlr4;

char* g_Optarg;
int g_Optind = 1, g_Opterr = 1, g_Optopt, g_Optpos, g_Optreset = 0;
int getopt(int argc, char* const argv[], const char* optstring);

int main(int argc, char* argv[])
{
	std::string inFilepath;
	std::string outFilepath;
	int c;
	while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c)
		{
		case 'i':
			inFilepath = (std::filesystem::current_path() / g_Optarg).string();
			std::cout << "Input file: " << inFilepath << std::endl;
			break;
		case 'o':
			outFilepath = (std::filesystem::current_path() / g_Optarg).string();
			std::cout << "Output file: " << outFilepath << std::endl;
			break;
		case '?':
			std::cerr << "Error: invalid option" << std::endl;
			return EXIT_FAILURE;
		case ':':
			std::cerr << "Error: missing argument" << std::endl;
			return EXIT_FAILURE;
		default: 
			std::cerr << "Error: Unknown" << std::endl;
		}
	}

	// Open the input file
	std::ifstream inFile(inFilepath);
	if (!inFile) {
		std::cerr << "Error: unable to open input file: " << inFilepath << std::endl;
		return EXIT_FAILURE;
	}
	// Read the contents of the input file into a stringstream
	std::stringstream inStream;
	inStream << inFile.rdbuf();
	
	ErrorLogger errorLogger{};
	//try
	//{
		ControlFlowGraph controlFlowGraph{false};
		ANTLRInputStream input(inStream.str());
		//ANTLRInputStream input("a + (x * (y ? 0 : 1) + z);");
		//ANTLRInputStream input("b+c*d; (b+c)*d;");
		//ANTLRInputStream input("void main()\n{\n\tint a = 1;\n\tint b = 3;\n\tint c = a + b;\n}");
		//ANTLRInputStream input("void main()\n{\n\tint a;\n\treturn a;\n}");
	//const prop test with if
	//ANTLRInputStream input("int main()\n{\n     int x = 2;\n     int y = x;\n     if(x)\n     {\n        x = 3;\n        if(x)\n          y = x;\n     }\n     return x;\n}");
	//////ANTLRInputStream input("int main()\n{\n     int x = 2;\n     int y;\n     int z = 10;\n     if(y)\n     {\n        x = 5+3;\n     }\n     else\n     {\n        z = 10;\n        y = -x;\n     }\n     return x+y;\n}");
	//ANTLRInputStream input("int main()\n{\n     int x = 2;\n     int y;\n     x = y + x;\n     if(x)\n     {\n        x = y;\n     }\n     return x;\n}");
		//ANTLRInputStream input("int test()\n{\n    int b = 2;\n    int c = 3;\n    int d = c+d;\n    return d;\n}\n\nvoid main()\n{\n    test();\n}");
		//ANTLRInputStream input("void test()\n{\n    char b = 'l';\n    int c = 3;\n    int d = c+d;\n    return;\n}\n\nvoid main()\n{\n    test();\n}");
		//ANTLRInputStream input("int func3(int x, int y, int z, int a, int b, int c, int d, int e, int h, int i, int j)\n{\n    int f = d + e + h + i + j;\n    int g = e;\n    int test = 5;\n    int test2 = 5;\n    int test3 = 5;\n\n    return b;\n}\n\nint main()\n{\n        int a = 2;\n        int b = 3;\n        int c = 3;\n        //int c = a + b;\n        int d = 5;\n        int e = 5;\n\n        func3(5,5,5,5,5,5,b,c,d,e,a);\n}");
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

		std::ofstream file(outFilepath, std::ios::out | std::ios::trunc);

		if (!file)
		{
			std::cerr << "Couldn't open file" << '\n';
			return EXIT_FAILURE;
		}

		file << stream.rdbuf();

		//std::cout << tree->toStringTree(&parser, true) << std::endl;
	//}
	//catch (...)
	//{
	//	errorLogger.Signal(ERROR, "Caught exception in main " + antlrcpp::what(), 0);
	//}

	return 0;
}

// Implementation of the unix getopt function from https://gist.github.com/attractivechaos/a574727fb687109a2adefcd75655d9ea
int getopt(int argc, char* const argv[], const char* optstring)
{
	int i, c, d;
	int k, l;
	char* optchar;

	if (!g_Optind || g_Optreset) {
		g_Optreset = 0;
		g_Optpos = 0;
		g_Optind = 1;
	}

	if (g_Optind >= argc || !argv[g_Optind])
		return -1;

	if (argv[g_Optind][0] != '-') {
		if (optstring[0] == '-') {
			g_Optarg = argv[g_Optind++];
			return 1;
		}
		return -1;
	}

	if (!argv[g_Optind][1])
		return -1;

	if (argv[g_Optind][1] == '-' && !argv[g_Optind][2])
		return g_Optind++, -1;

	if (!g_Optpos) g_Optpos++;
	c = argv[g_Optind][g_Optpos], k = 1;
	optchar = argv[g_Optind] + g_Optpos;
	g_Optopt = c;
	g_Optpos += k;

	if (!argv[g_Optind][g_Optpos]) {
		g_Optind++;
		g_Optpos = 0;
	}

	if (optstring[0] == '-' || optstring[0] == '+')
		optstring++;

	i = 0;
	d = 0;
	do {
		d = optstring[i], l = 1;
		if (l > 0) i += l; else i++;
	} while (l && d != c);

	if (d != c) {
		if (optstring[0] != ':' && g_Opterr)
			std::cerr << argv[0] << ": unrecognized option: " << optchar << k;
		return '?';
	}
	if (optstring[i] == ':') {
		if (optstring[i + 1] == ':') g_Optarg = 0;
		else if (g_Optind >= argc) {
			if (optstring[0] == ':') return ':';
			if (g_Opterr) std::cerr << argv[0] <<
				": option requires an argument: " <<
				optchar << k;
			return '?';
		}
		if (optstring[i + 1] != ':' || g_Optpos) {
			g_Optarg = argv[g_Optind++] + g_Optpos;
			g_Optpos = 0;
		}
	}
	return c;
}
