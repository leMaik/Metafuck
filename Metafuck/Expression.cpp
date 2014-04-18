#include "Expression.h"

Expression::Expression(std::string code) : Statement(code)
{
}

std::string Expression::compile(Compiler& cmp, Brainfuck& bf){
	return "";
}

bool Expression::matches(std::string code)
{
	return false; //Expression is not implemented => will never match
}