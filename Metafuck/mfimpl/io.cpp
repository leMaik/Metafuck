#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"
#include "../String.h"

void metafuck::impl::io::print(const Call& c, Compiler& compiler, Brainfuck& bf) {
	switch (c.signature().second[0]) {
	case Argument::STRING:
		compiler.generated_ << bf.printString(c.arg<String>(0).getValue());
		break;
	case Argument::VARIABLE:
		compiler.generated_ << bf.print(compiler.getVar(c.arg<Variable>(0).getName()));
		break;
	default:
		return;
	}
}