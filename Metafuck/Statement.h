#ifndef STATEMENT_H
#define STATEMENT_H

#include "Argument.h"
#include <string>

class Compiler;
class Brainfuck;

class Statement
{
public:
	Statement() = default;
	virtual ~Statement() = default;

	virtual std::string compile(Compiler& cmp, Brainfuck& bf) = 0;
	virtual std::string toString() const = 0;
	virtual bool returns() const = 0;
	virtual unsigned int result() = 0;

private:
	unsigned int resultIndex;
};

#endif