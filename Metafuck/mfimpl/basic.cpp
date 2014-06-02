#include "basic.h"
#include "../compiler.h"

void metafuck::impl::basic::set(Compiler& compiler, const Variable& var, const Evaluatable& value) {
	unsigned int varCell = compiler.getVar(var);
	value.evaluate(compiler, varCell);
}

void metafuck::impl::basic::let(Compiler& compiler, const Variable& var, const Evaluatable& value) {
	compiler.setVar(var, value.evaluate(compiler));
}