//#define BenchmarkMain
// I know this switching between main is terrible, its just cause of the lack of time

#ifndef  BenchmarkMain
#include <iostream>
#include <filesystem>

#include "antlr4-runtime.h"
#include "CompilerFlags.hpp"
#include "CricketLexer.h"
#include "CricketParser.h"

#include "Scope.h"
#include "ErrorHandeler.h"
#include "Intermediate/CodeVisitor.h"
#include "Intermediate/ControlFlowGraph.h"

using namespace antlrcpp;
using namespace antlr4;

// Gross globals I know
char* g_Optarg;
int g_Optind = 1, g_Opterr = 1, g_Optopt, g_Optpos, g_Optreset = 0;
int getopt(int argc, char* const argv[], const char* optstring);
void handleCompilerFlag(const std::string& flag, bool state, std::string& errorlog);
void exitFailure(std::ostream& file);
void setAllFlags(bool state);

// I know this main function is a bit mess but I promise the other code is better
int main(int argc, char* argv[])
{
#pragma region mainSetup
	// Read the given flags
	std::string inFilepath;
	std::string outFilepath;
	std::string errorFilepath{};// = (std::filesystem::current_path() / "Errorlog.txt").string();
	std::string savedErrors{};
	int c;
	while ((c = getopt(argc, argv, "i:o:O:f:n:e:")) != -1) {
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
		case 'f':
			handleCompilerFlag(g_Optarg, true, savedErrors);
			break;
		case 'n':
			handleCompilerFlag(g_Optarg, false, savedErrors);
			break;
		case 'e':
			errorFilepath = (std::filesystem::current_path() / g_Optarg).string();
			std::cout << "Error output file: " << errorFilepath << std::endl;
			break;
		case 'O':
			if (g_Optarg[0] == '0')
				setAllFlags(false);
			else
				setAllFlags(true);
			break;
		case '?':
			savedErrors += "Error: invalid compiler flag option\n";
			break;
		case ':':
			savedErrors += "Error: missing compiler flag argument\n";
			break;
		default:
			savedErrors += "Error: Unknown compiler flag\n";
			break;
		}
	}

	if (!errorFilepath.empty())
		freopen(errorFilepath.c_str(), "w", stderr);
#ifdef _DEBUG
	std::cerr << savedErrors + " argc: " + std::to_string(argc) + '\n';
#endif

	// Open output file
	std::ofstream outFile(outFilepath, std::ios::out | std::ios::trunc);
	if (!outFile)
	{
		std::cerr << "Error: Couldn't open output file: " << outFilepath << '\n';
		exitFailure(outFile);
	}
	// Open the input file
	std::ifstream inFile(inFilepath);
	if (!inFile) {
		std::cerr << "Error: Unable to open input file: " << inFilepath << '\n';
		return EXIT_FAILURE;
	}
#pragma endregion mainSetup

	// Read the contents of the input file into a stringstream
	std::stringstream inStream;
	inStream << inFile.rdbuf();

	ANTLRInputStream input(inStream.str());

	CricketLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	CricketParser parser(&tokens);
	tree::ParseTree* tree = parser.start();

	if (parser.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while parsing\n";
		exitFailure(outFile);
	}
	if (lexer.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while scanning\n";
		exitFailure(outFile);
	}

	ErrorLogger errorLogger{};
	ControlFlowGraph controlFlowGraph{};
	CodeVisitor codeVisitor{ errorLogger, controlFlowGraph };
	codeVisitor.visit(tree);

	//Check for errors after visiting the code
	if (errorLogger.HasError())
		exitFailure(outFile);

	std::stringstream stream;
	controlFlowGraph.GenerateX86(stream);

	// Save the generated assembly in the output file
	outFile << stream.rdbuf();

	return 0;
}

void handleCompilerFlag(const std::string& flag, bool state, std::string& errorlog)
{
	if (flag == "verbose-asm")
	{
		compilerFlags::g_AddComents = state;
	}
	else if (flag == "peephole")
	{
		compilerFlags::g_PeepholeOptimization = state;
	}
	else if (flag == "regalloc")
	{
		compilerFlags::g_RemoveTempVars = state;
		if (state)
			compilerFlags::g_PeepholeOptimization = state;
	}
	else if (flag == "dce")
	{
		compilerFlags::g_RemoveDeadcode = state;
		compilerFlags::g_RemoveConstConditionals = state;
	}
	else if (flag == "cprop")
	{
		compilerFlags::g_OptimizeConstPropagation = state;
	}
	else
	{
		errorlog += "Warning: Unknown flag " + flag;
	}
}

void setAllFlags(bool state)
{
	//compilerFlags::g_AddComents = state;
	compilerFlags::g_PeepholeOptimization = state;
	compilerFlags::g_RemoveTempVars = state;
	compilerFlags::g_RemoveDeadcode = state;
	compilerFlags::g_RemoveConstConditionals = state;
	compilerFlags::g_OptimizeConstPropagation = state;
}

void exitFailure(std::ostream& file)
{
	file << "Compilation Errors -> see errorlog.txt";
	exit(EXIT_FAILURE);
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

#elif defined(BenchmarkMain) // ^^^ !BECNHMARKMAIN ^^^ // vvv BENCHMARKMAIN vvv

#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <concepts>
#include <numeric>

#include "antlr4-runtime.h"
#include "CompilerFlags.hpp"
#include "CricketLexer.h"
#include "CricketParser.h"

#include "Scope.h"
#include "ErrorHandeler.h"
#include "benchmark.hpp"
#include "Intermediate/CodeVisitor.h"
#include "Intermediate/ControlFlowGraph.h"

//#define MEASURE_MEMORY_USAGE

using namespace antlrcpp;
using namespace antlr4;

// Gross globals I know
char* g_Optarg;
int g_Optind = 1, g_Opterr = 1, g_Optopt, g_Optpos, g_Optreset = 0;
int getopt(int argc, char* const argv[], const char* optstring);
void handleCompilerFlag(const std::string& flag, bool state, std::string& errorlog);
void exitFailure(std::ostream& file);
void setAllFlags(bool state);
void formatMemUsageTable(const std::string& text, size_t val);

// Don't mind the meta programming ;)
template<typename T>
concept OutputStreamable = requires(T a, std::ostream & os) { { os << a } -> std::same_as<std::ostream&>; }; // Could probably be just done with type traits std::is_output_streamable but this is cooler
template<OutputStreamable T, OutputStreamable... Args>
void writeToCSV(std::ofstream& file, T first, Args... args);
void compilerMain(int argc, char* argv[]);

//I know this is done rather quick and dirty
int main(int argc, char* argv[])
{
	std::vector<std::chrono::microseconds> times;
	constexpr int iterations = 5000;
	for (size_t i{ 0 }; i < iterations; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		compilerMain(argc, argv);
		auto end = std::chrono::high_resolution_clock::now();
		times.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start));
		g_Optreset = 1;
	}

	// Sort the times
	std::sort(times.begin(), times.end());

	// Discard the highest and lowest values
	times.erase(times.begin(), std::next(times.begin(), iterations/10));
	times.erase(std::next(times.rbegin(), iterations/10).base(), times.end());

	// Calculate the average
	const std::chrono::microseconds totalTime = std::accumulate(times.begin(), times.end(), std::chrono::microseconds(0));

	//std::cout << "Average time taken: " << totalTime.count() / times.size() << " microseconds" << std::endl;
	auto avgTime = (double(totalTime.count()) / double(times.size())) / 1'000'000.0;
	std::cout << "Average time taken: " << avgTime << " seconds" << std::endl;

	std::ofstream excelFile("../../Benchmark/Output/BenchResultCrk.csv", std::ios::out | std::ios::app);
	if (!excelFile)
		std::cerr << "Error: Couldn't open output file: " << "../../Benchmark/Output/BenchResultCrk.csv" << '\n';
	else
		writeToCSV(excelFile, argv[5], avgTime);

	return 0;
}

void compilerMain(int argc, char* argv[])
{
#pragma region mainSetup
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemState stateSetup1, stateSetup2, stateSetup3{};
	_CrtMemCheckpoint(&stateSetup1);
#endif

	// Read the given flags
	std::string inFilepath;
	std::string outFilepath;
	std::string errorFilepath{};// = (std::filesystem::current_path() / "Errorlog.txt").string();
	std::string savedErrors{};
	int c;
	while ((c = getopt(argc, argv, "i:o:O:f:n:e:")) != -1) {
		switch (c)
		{
		case 'i':
			inFilepath = (std::filesystem::current_path() / g_Optarg).string();
			//std::cout << "Input file: " << inFilepath << std::endl;
			break;
		case 'o':
			outFilepath = (std::filesystem::current_path() / g_Optarg).string();
			//std::cout << "Output file: " << outFilepath << std::endl;
			break;
		case 'f':
			handleCompilerFlag(g_Optarg, true, savedErrors);
			break;
		case 'n':
			handleCompilerFlag(g_Optarg, false, savedErrors);
			break;
		case 'e':
			errorFilepath = (std::filesystem::current_path() / g_Optarg).string();
			//std::cout << "Error output file: " << errorFilepath << std::endl;
			break;
		case 'O':
			if (g_Optarg[0] == '0')
				setAllFlags(false);
			else
				setAllFlags(true);
			break;
		case '?':
			savedErrors += "Error: invalid compiler flag option\n";
			break;
		case ':':
			savedErrors += "Error: missing compiler flag argument\n";
			break;
		default:
			savedErrors += "Error: Unknown compiler flag\n";
			break;
		}
	}

	if (!errorFilepath.empty())
		freopen(errorFilepath.c_str(), "w", stderr);
#ifdef _DEBUG
	std::cerr << savedErrors + " argc: " + std::to_string(argc) + '\n';
#endif

	// Open output file
	std::ofstream outFile(outFilepath, std::ios::out | std::ios::trunc);
	if (!outFile)
	{
		std::cerr << "Error: Couldn't open output file: " << outFilepath << '\n';
		exitFailure(outFile);
	}
	// Open the input file
	std::ifstream inFile(inFilepath);
	if (!inFile) {
		std::cerr << "Error: Unable to open input file: " << inFilepath << '\n';
		exit(EXIT_FAILURE);
	}
#pragma endregion mainSetup

	// Read the contents of the input file into a stringstream
	std::stringstream inStream;
	inStream << inFile.rdbuf();

	ANTLRInputStream input(inStream.str());

#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemCheckpoint(&stateSetup2);
	_CrtMemDifference(&stateSetup3, &stateSetup1, &stateSetup2);
#endif

	// Start Memory measurements FrontEnd
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemState stateFE1, stateFE2, stateFE3{};
	_CrtMemCheckpoint(&stateFE1);
#endif
	CricketLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	CricketParser parser(&tokens);
	tree::ParseTree* tree = parser.start();
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemCheckpoint(&stateFE2);
	_CrtMemDifference(&stateFE3, &stateFE1, &stateFE2);
#endif
	// End Memory measurements FrontEnd

	if (parser.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while parsing\n";
		exitFailure(outFile);
	}
	if (lexer.getNumberOfSyntaxErrors() != 0)
	{
		std::cerr << "ERROR: syntax error(s) while scanning\n";
		exitFailure(outFile);
	}

	// Start Memory measurements IntermediateRepresentation
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemState state1, state2, state3{};
	_CrtMemCheckpoint(&state1);
#endif
	ErrorLogger errorLogger{};
	ControlFlowGraph controlFlowGraph{};
	CodeVisitor codeVisitor{ errorLogger, controlFlowGraph };
	codeVisitor.visit(tree);
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemCheckpoint(&state2);
	_CrtMemDifference(&state3, &state1, &state2);
#endif
	// End Memory measurements IntermediateRepresentation

		//Check for errors after visiting the code
	if (errorLogger.HasError())
		exitFailure(outFile);

	// Start Memory measurements CodeGeneration
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemState state21, state22, state23{};
	_CrtMemCheckpoint(&state21);
#endif
	std::stringstream stream;
	controlFlowGraph.GenerateX86(stream);
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	_CrtMemCheckpoint(&state22);
	_CrtMemDifference(&state23, &state21, &state22);
#endif
	// End Memory measurements CodeGeneration

	// Format results
#if defined(_DEBUG) && defined(MEASURE_MEMORY_USAGE)
	//std::ofstream excelFile("../Output/MemResult.csv", std::ios::out | std::ios::app);
	//writeToCSV(excelFile, "mem usage", "size");
	//excelFile << '\n';

	std::cout << "Memory usages: \n";
	formatMemUsageTable("setup phase ", stateSetup3.lHighWaterCount);
	formatMemUsageTable("Antlr's Front end ", stateFE3.lHighWaterCount);
	formatMemUsageTable("intermediate representaion ", state3.lHighWaterCount);
	formatMemUsageTable("code gen ", state23.lHighWaterCount);
	formatMemUsageTable("Total ", stateSetup3.lHighWaterCount + stateFE3.lHighWaterCount + state3.lHighWaterCount + state23.lHighWaterCount);
#endif

	// Save the generated assembly in the output file
	outFile << stream.rdbuf();
}

void handleCompilerFlag(const std::string& flag, bool state, std::string& errorlog)
{
	if (flag == "verbose-asm")
	{
		compilerFlags::g_AddComents = state;
	}
	else if (flag == "peephole")
	{
		compilerFlags::g_PeepholeOptimization = state;
	}
	else if (flag == "regalloc")
	{
		compilerFlags::g_RemoveTempVars = state;
		if (state)
			compilerFlags::g_PeepholeOptimization = state;
	}
	else if (flag == "dce")
	{
		compilerFlags::g_RemoveDeadcode = state;
		compilerFlags::g_RemoveConstConditionals = state;
	}
	else if (flag == "cprop")
	{
		compilerFlags::g_OptimizeConstPropagation = state;
	}
	else
	{
		errorlog += "Warning: Unknown flag " + flag;
	}
}

// I really wanted to at least use one meta programming function as i love meta programming; I know this isn't very readable but comon :)
template<OutputStreamable T, OutputStreamable... Args>
void writeToCSV(std::ofstream& file, T first, Args... args)
{
	file << first;
	if constexpr (sizeof...(args) > 0) {
		file << ",";
		writeToCSV(file, args...);
	}
	else
		file << '\n';
}

void setAllFlags(bool state)
{
	//compilerFlags::g_AddComents = state;
	compilerFlags::g_PeepholeOptimization = state;
	compilerFlags::g_RemoveTempVars = state;
	compilerFlags::g_RemoveDeadcode = state;
	compilerFlags::g_RemoveConstConditionals = state;
	compilerFlags::g_OptimizeConstPropagation = state;
}

//https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c
void formatMemUsageTable(const std::string& text, size_t val)
{
	std::cout << std::format("{0:<28} : {1:>7}kb\n", text, val / 100.f);
}

void exitFailure(std::ostream& file)
{
	file << "Compilation Errors -> see errorlog.txt";
	exit(EXIT_FAILURE);
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

#endif // ^^^ BECNHMARKMAIN ^^^
