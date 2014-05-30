#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"

void metafuck::impl::io::print_str(Compiler &compiler, const String& str) {
	compiler.generated_ << compiler.bf().printString(str.getValue());
}

void metafuck::impl::io::print_var(Compiler &compiler, const Evaluatable& var) {
	compiler.generated_ << compiler.bf().print(var.evaluate(compiler));
}

void metafuck::impl::io::getchar(Compiler &compiler, const Variable& var) {
	compiler.generated_ << compiler.bf().input(compiler.getVar(var));
}
