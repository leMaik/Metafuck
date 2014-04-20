#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../Procedure.h"
#include "../compiler.h"

class Print : public Procedure
{
public:
	Print(std::string code);
	std::string compile(Compiler& cmp, Brainfuck& bf);
	static bool matches(std::string code);
};

#endif