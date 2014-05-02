#ifndef CALLSTATEMENT_H
#define CALLSTATEMENT_H

#include "Statement.h"
#include "compiler.h"
#include <functional>

class CallStatement : public Statement
{
public:
	CallStatement(MfProcedure procedure, const Call& call);

	std::string compile(Compiler& cmp, Brainfuck& bf);
	std::string toString() const;
	bool returns() const;
	unsigned int result();

private:
	MfProcedure procedure_;
	const Call& call_;
};

#endif
