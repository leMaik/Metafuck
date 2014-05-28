#include "logic.h"

unsigned int metafuck::impl::logic::iseq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().isEqual(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::isnoteq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().isNotEqual(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::and(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().logicalAnd(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::or(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb) {
	compiler.generated_ << compiler.bf().logicalOr(var.evaluate(compiler), varb.evaluate(compiler), result);
	return result;
}

unsigned int metafuck::impl::logic::not(Compiler &compiler, unsigned int result, const Evaluatable& var) {
	compiler.generated_ << compiler.bf().isNot(var.evaluate(compiler), result);
	return result;
}