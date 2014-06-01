#include "logic.h"

unsigned int metafuck::impl::logic::iseq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().isEqual(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::isnoteq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().isNotEqual(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::and_fn(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().logicalAnd(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::or_fn(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().logicalOr(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::not_fn(Compiler &compiler, unsigned int result, const Evaluatable& var) {
	compiler.generated_ << compiler.bf().isNot(var.evaluate(compiler), result);
	return result;
}
