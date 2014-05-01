#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"
#include "../String.h"

void metafuck::impl::io::print(const Call& c, Compiler& compiler, Brainfuck& bf) {
	compiler.generated_ << bf.printString(c.arg<String>(0).getValue());
}