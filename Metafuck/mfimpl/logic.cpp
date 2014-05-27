#include "logic.h"

unsigned int metafuck::impl::logic::iseq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().isEqual(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}
