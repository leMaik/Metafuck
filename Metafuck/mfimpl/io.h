#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../Procedure.h"
#include "../compiler.h"
#include "../Call.h"

class Print : public Procedure
{
public:
	Print(Call const& call);
	std::string compile(Compiler& cmp, Brainfuck& bf);
	static bool matches(Call const& call);
};

#endif