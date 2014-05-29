#include "math.h"

void metafuck::impl::math::add_const(Compiler& compiler, const Variable& var, const Number& c) {
	compiler.generated_ << compiler.bf().add(compiler.getVar(var), c.getValue());
}

void metafuck::impl::math::add_ev(Compiler& compiler, const Variable& var, const Evaluatable& c) {
	compiler.generated_ << compiler.bf().addCellTo(compiler.getVar(var), c.evaluate(compiler), compiler.getVar(var));
}

void metafuck::impl::math::sub_const(Compiler& compiler, const Variable& var, const Number& c) {
	compiler.generated_ << compiler.bf().sub(compiler.getVar(var), c.getValue());
}

void metafuck::impl::math::sub_ev(Compiler& compiler, const Variable& var, const Evaluatable& c) {
	compiler.generated_ << compiler.bf().subCellFrom(compiler.getVar(var), c.evaluate(compiler), compiler.getVar(var));
}

unsigned int metafuck::impl::math::div(Compiler& compiler, unsigned int target, const Evaluatable& divident, const Evaluatable& divisor) {
	unsigned int cells = compiler.bf().allocCell(2);
	compiler.bf().maketemp(cells + 1);
	compiler.generated_ << compiler.bf().divmod(divident.evaluate(compiler), divisor.evaluate(compiler), cells);
	return cells;
}

unsigned int metafuck::impl::math::mod(Compiler& compiler, unsigned int target, const Evaluatable& divident, const Evaluatable& divisor) {
	unsigned int cells = compiler.bf().allocCell(2);
	compiler.bf().maketemp(cells);
	compiler.generated_ << compiler.bf().divmod(divident.evaluate(compiler), divisor.evaluate(compiler), cells);
	return cells + 1;
}
