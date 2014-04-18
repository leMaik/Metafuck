#ifndef STATEMENT_H
#define STATEMENT_H

#include "brainfuck.h"
#include "compiler.h"
#include <string>

class Statement
{
public:
	Statement(std::string code);
	virtual ~Statement() = default;

	virtual std::string compile(Compiler& cmp, Brainfuck& bf) = 0;
	virtual std::string toString() const = 0;

protected:
	std::string code_;
};

#endif