#include "basic.h"
#include "../compiler.h"

void metafuck::impl::basic::set(Compiler& compiler, const Variable& var, const Evaluatable& value) {
	unsigned int varCell = compiler.getVar(var, true);
	value.evaluate(compiler, varCell);
}
