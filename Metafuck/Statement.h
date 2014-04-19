#ifndef STATEMENT_H
#define STATEMENT_H

#include "Argument.h"
#include <string>

class Compiler;
class Brainfuck;

class Statement : public Argument
{
public:
	Statement(std::string code);

	virtual std::string compile(Compiler& cmp, Brainfuck& bf) = 0;
	virtual std::string toString() const = 0;
	virtual bool returns() const = 0;
	virtual Argument::Type getType() const = 0;

protected:
	std::string code_;
};

#endif