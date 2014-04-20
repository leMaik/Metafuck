#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"
#include "../String.h"

Print::Print(std::string code) : Procedure(code)
{

}

std::string Print::compile(Compiler& cmp, Brainfuck& bf)
{
	return bf.printString(arg<String>(0).getValue());
}

bool Print::matches(std::string code)
{
	return true;
}
