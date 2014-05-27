#include "flow.h"
#include "../CallList.h"

void metafuck::impl::flow::if_fn(const Call& c) {
	///*unsigned int x = bf_.allocCell();
	//evaluateTo(c.arg(0), x);
	//generated_ << bf_.move(x) << "[";
	//c.arg<CallList>(1).compile(*this, bf_);
	//generated_ << bf_.set(x, 0) << "]";*/
}

void metafuck::impl::flow::if_else_fn(const Call& c) {
	/*unsigned int temp0 = bf_.allocCell();
	unsigned int temp1 = bf_.allocCell();
	evaluateTo(c.arg(0), temp1);
	generated_ << bf_.set(temp0, 1);
	generated_ << bf_.move(temp1) << "[";
	c.arg<CallList>(1).compile(*this, bf_);
	generated_ << bf_.move(temp0) << "-";
	generated_ << bf_.set(temp1, 0) << "]";
	generated_ << bf_.move(temp0) << "[";
	c.arg<CallList>(2).compile(*this, bf_);
	generated_ << bf_.move(temp0) << "-]";
	bf_.freeCell(temp0);
	bf_.freeCell(temp1);*/
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