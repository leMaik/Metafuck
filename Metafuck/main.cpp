#include "compiler.h"
#include "brainfuck.h"
#include "Interpreter\ConvenienceFuck.h"
#include <iostream>
#include <string>
#include <stack>

#define STACK_SIZE 42

using namespace std;

void pause() {
	cin.clear();
	cin.get();
}

int main(void)
{
	string input;
	getline(cin, input);
	Compiler com(input);
	if (!com.validate()) {
		cerr << "ERROR: Your code is invalid. Check ALL the brackets!\n";
		return -1;
	}
	cout << "Compiling...\n";
	cout << com.lex() << " statements found.\n";
	com.compile();
	//com.optimize();
	cout << "===\n";
	cout << com.getGeneratedCode() << "\n";
	cout << "===\nRunning:\n";
	RunBrainfuckProgram(com.getGeneratedCode());

	pause();

	return 0;
}