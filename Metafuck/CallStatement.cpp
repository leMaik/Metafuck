#include "CallStatement.h"


CallStatement::CallStatement(MfProcedure procedure, const Call& call) : procedure_(procedure), call_(call)
{
}

std::string CallStatement::compile(Compiler& cmp, Brainfuck& bf) {
	procedure_(call_, cmp, bf);
	return ""; //TODO
}

std::string CallStatement::toString() const {
	return call_.signature().first;
}

bool CallStatement::returns() const {
	return false;
}

unsigned int CallStatement::result() {
	return -1;
}