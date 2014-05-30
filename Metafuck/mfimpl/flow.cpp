#include "flow.h"
#include "../CallList.h"
#include "../compiler.h"

void metafuck::impl::flow::if_fn(Compiler &compiler, const Evaluatable& var, const Call& doif) {
	auto x = compiler.bf().maketemp(var.evaluate(compiler));
	compiler.generated_ << compiler.bf().move(x) << "[";
	doif.compile(compiler);
	compiler.generated_ << compiler.bf().set(x, 0) << "]";
}

void metafuck::impl::flow::if_else_fn(Compiler &compiler, const Evaluatable& var, const Call& doif, const Call& doelse) {
	auto temp0 = compiler.bf().allocCell_t();
	auto temp1 = compiler.bf().maketemp(var.evaluate(compiler));
	compiler.generated_ << compiler.bf().set(temp0, 1);
	compiler.generated_ << compiler.bf().move(temp1) << "[";
	doif.compile(compiler);
	compiler.generated_ << compiler.bf().move(temp0) << "-";
	compiler.generated_ << compiler.bf().set(temp1, 0) << "]";
	compiler.generated_ << compiler.bf().move(temp0) << "[";
	doelse.compile(compiler);
	compiler.generated_ << compiler.bf().move(temp0) << "-]";
}

void metafuck::impl::flow::while_fn(Compiler &compiler, const Evaluatable& var, const Call& inner) {
	auto temp = compiler.bf().maketemp(var.evaluate(compiler));
	compiler.generated_ << compiler.bf().move(temp) << "[";
	inner.compile(compiler);
	var.evaluate(compiler, temp);
	compiler.generated_ << compiler.bf().move(temp) << "]";
}

void metafuck::impl::flow::do_while_fn(Compiler &compiler, const Call& inner, const Evaluatable& var) {
	unsigned int temp = compiler.bf().allocCell_t();
	compiler.generated_ << compiler.bf().set(temp, 1);
	compiler.generated_ << compiler.bf().move(temp) << "[";
	inner.compile(compiler);
	var.evaluate(compiler, temp);
	compiler.generated_ << compiler.bf().move(temp) << "]";
}

void metafuck::impl::flow::for_fn(Compiler &compiler, const Variable& var, const Evaluatable& init, const Evaluatable& condition, const Call& inner, const Call& step) {
    //TODO broken!!

    //TODO: scopes, for inner and for iteration variable
	init.evaluate(compiler, compiler.getVar(var, true)); //set iteration variable to start value

	////now, it's similar to while_fn...
	auto temp = compiler.bf().allocCell_t();
	condition.evaluate(compiler, temp);
	compiler.generated_ << compiler.bf().move(temp) << "[";
	inner.compile(compiler);
	step.compile(compiler);
	condition.evaluate(compiler, temp);
	compiler.generated_ << compiler.bf().move(temp) << "]";
}
