#include "flow.h"
#include "../CallList.h"
#include "../compiler.h"

void metafuck::impl::flow::if_fn(Compiler &compiler, const Evaluatable& var, const Call& doif) {
	unsigned int x = var.evaluate(compiler);
	compiler.generated_ << compiler.bf().move(x) << "[";
	doif.compile(compiler);
	compiler.generated_ << compiler.bf().set(x, 0) << "]";
	compiler.bf().freeCell(x);
}

void metafuck::impl::flow::if_else_fn(Compiler &compiler, const Evaluatable& var, const Call& doif, const Call& doelse) {
	unsigned int temp0 = compiler.bf().allocCell();
	unsigned int temp1 = var.evaluate(compiler);
	compiler.generated_ << compiler.bf().set(temp0, 1);
	compiler.generated_ << compiler.bf().move(temp1) << "[";
	doif.compile(compiler);
	compiler.generated_ << compiler.bf().move(temp0) << "-";
	compiler.generated_ << compiler.bf().set(temp1, 0) << "]";
	compiler.generated_ << compiler.bf().move(temp0) << "[";
	doelse.compile(compiler);
	compiler.generated_ << compiler.bf().move(temp0) << "-]";
	compiler.bf().freeCell(temp0);
	compiler.bf().freeCell(temp1);
}

void metafuck::impl::flow::while_fn(const Call& c) {
	/*unsigned int temp = evaluateTo(c.arg(0));
	generated_ << bf_.move(temp) << "[";
	c.arg<Statement>(1).compile(*this, bf_);
	evaluateTo(c.arg(0), temp);
	generated_ << bf_.move(temp) << "]";
	if (c.arg(0).getType() == Argument::CALL) {
	bf_.freeCell(temp);
	}*/
}

void metafuck::impl::flow::do_while_fn(const Call& c) {
	/*unsigned int temp = bf_.allocCell();
	generated_ << bf_.set(temp, 1);
	generated_ << bf_.move(temp) << "[";
	c.arg<Statement>(0).compile(*this, bf_);
	evaluateTo(c.arg(1), temp);
	generated_ << bf_.move(temp) << "]";
	if (c.arg(1).getType() == Argument::CALL) {
	bf_.freeCell(temp);
	}*/
}

void metafuck::impl::flow::for_fn(const Call& c) {
	//unsigned int var = getVar(c.arg<Variable>(0));
	//set(var, c.arg(1)); //set iteration variable to start value

	////now, it's similar to while_fn...
	//unsigned int temp = bf_.allocCell();
	//evaluateTo(c.arg(2), temp);
	//generated_ << bf_.move(temp) << "[";
	//c.arg<Statement>(4).compile(*this, bf_); //code in the loop
	//c.arg<Statement>(3).compile(*this, bf_); //step
	//evaluateTo(c.arg(2), temp);
	//generated_ << bf_.move(temp) << "]";

	//bf_.freeCell(temp);
}
