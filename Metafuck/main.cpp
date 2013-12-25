#include "compiler.h"
#include "brainfuck.h"
#include "Interpreter\ConvenienceFuck.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>

void pause() {
	std::cin.clear();
	std::cin.get();
}

int main(int argc, char** argv)
{
	std::string input;
	if (argc >= 2){
		std::ifstream in;
		in.open(argv[1]);
		std::stringstream strStream;
		strStream << in.rdbuf();//read the file
		input = strStream.str();
		in.close();
	}
	else {
		std::getline(std::cin, input);
	}
	bool shallRun = false;

	Compiler com(input);
	com.lex();
	com.compile();
	//com.optimize();

	if (argc >= 3 && argv[2]) {
		std::ofstream out;
		out.open(argv[2]);
		out << com.getGeneratedCode();
		out.close();
	}
	else {
		std::cout << com.getGeneratedCode() << std::endl;
	}

	if (shallRun) {
		std::cout << "===" << std::endl << "Running:" << std::endl;
		RunBrainfuckProgram(com.getGeneratedCode());
		std::cout << std::endl << "===" << std::endl;;
		pause();
	}
	std::cout << "Done. :)";
	return 0;
}