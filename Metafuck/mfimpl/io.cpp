#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"

#define STRINGIFY(x) #x

void metafuck::impl::io::print_str(Compiler &compiler, const String& str) {
	compiler.generated_ << compiler.bf().printString(str.getValue());
}

void metafuck::impl::io::print_var(Compiler &compiler, const Evaluatable& var) {
	compiler.generated_ << compiler.bf().print(var.evaluate(compiler));
}

void metafuck::impl::io::print_num(Compiler &compiler, const Evaluatable& var) {
	compiler.scope();
	compiler.setVar(Variable("__ARG__"), var.evaluate(compiler));
	CallList(
#include "printn.txt"
		).compile(compiler);
}

void metafuck::impl::io::getchar(Compiler &compiler, const Variable& var) {
	compiler.generated_ << compiler.bf().input(compiler.getVar(var));
}
