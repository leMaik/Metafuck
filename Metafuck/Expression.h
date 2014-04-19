#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Statement.h"
#include "compiler.h"
#include <string>

class Expression : public Statement
{
public:
	Expression(std::string code);
	virtual std::string compile(Compiler& cmp, Brainfuck& bf);
	static bool matches(std::string code);
};

#endif