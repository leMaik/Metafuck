#include "io.h"
#include "../compiler.h"
#include "../brainfuck.h"
#include "../String.h"

void metafuck::impl::io::print_str(Compiler& compiler, String& str) {
	compiler.generated_ << compiler.bf().printString(str.getValue());
}

void metafuck::impl::io::print_var(Compiler& compiler, Variable& var) {
	compiler.generated_ << compiler.bf().print(compiler.getVar(var.getName()));
}

void metafuck::impl::io::getchar(Compiler& compiler, Variable& var) {
	compiler.generated_ << compiler.bf().input(compiler.getVar(var));
}