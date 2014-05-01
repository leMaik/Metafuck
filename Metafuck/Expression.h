#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Statement.h"
#include "compiler.h"
#include <string>

class Expression : public Statement
{
public:
	Expression(std::string code);

	std::string compile(Compiler& cmp, Brainfuck& bf);
	static bool matches(std::string code);

	std::string toString() const;
	bool returns() const;
};

#endif