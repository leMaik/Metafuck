#include "cell.h"
#include "brainfuck.h"
#include "Call.h"
#include "CallList.h"
#include "Variable.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <map>

class Compiler
{
private:
	Brainfuck bf_;
	std::string code_;
	std::stringstream generated_;
	CallList lexed_;
	std::map<std::string, unsigned int> vars_;

	unsigned int getVar(const Variable& variable, Brainfuck &b);
	bool isNumber(const std::string &s) const;
	bool isString(const std::string &s) const;
	void evaluate(Argument& arg);

public:
	bool validate();
	std::size_t lex();
	void compile();
	std::string getCode() const;
	std::string getGeneratedCode() const;

	Compiler(std::string c);
};
