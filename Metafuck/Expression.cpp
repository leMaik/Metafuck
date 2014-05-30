#include "Expression.h"

Expression::Expression(std::string code)
{
}

std::string Expression::compile(Compiler& cmp, Brainfuck& bf){
	return ".";
}

std::string Expression::toString() const
{
	return "<expression>"; //TODo
}

bool Expression::returns() const
{
	return true; //Expressions always return something
}