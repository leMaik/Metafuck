#include "cell.h"
#include "brainfuck.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <map>

class Compiler
{
private:
	std::string code_;
	std::stringstream generated_;
	std::vector<std::vector<std::string>> lexed_;
	std::map<std::string, unsigned int> vars_;

	unsigned int getVar(const std::string &name, Brainfuck &b);
	bool isNumber(const std::string &s) const;

	template <typename T>
	T pop(std::stack<T> &s);

public:
	bool validate();
	std::size_t lex();
	void compile();
	std::string getCode() const;
	std::string getGeneratedCode() const;

	Compiler(std::string c);
};

template <typename T>
T Compiler::pop(std::stack<T> &s) {
	T l = s.top();
	s.pop();
	return l;
}
