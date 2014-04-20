#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"
#include "../String.h"

Print::Print(Call const& call) : Procedure(call){}

std::string Print::compile(Compiler& cmp, Brainfuck& bf)
{
	return bf.printString(arg<String>(0).getValue());
}

bool Print::matches(Call const& call) //TODO use CallSignature here
{
	return call.getFunction() == "print" && call.arg(0).getType() == Argument::STRING;
}
