#include "array.h"

void metafuck::impl::array::init(Compiler& cmp, const Variable& var, const Number& length) {
	if (length.getValue() > 256)
		cmp.error(&var, "Arrays with a length of more than 256 are not supported by Metafuck");
	unsigned int result = cmp.bf().initArray(length.getValue());
	cmp.generated_ << cmp.bf().set(result, 0);
	cmp.setVar(var.getName(), result);
}

void metafuck::impl::array::set(Compiler& cmp, const Variable& var, const Evaluatable& index, const Evaluatable& value) {
	if (index.getType() == Type::INTEGER){
		auto target = cmp.bf().getArrayPointer(cmp.getVar(var), static_cast<const Number&>(index).getValue());
		value.evaluate(cmp, target);
	}
	else {
		cmp.generated_ << cmp.bf().arraySet(cmp.getVar(var), index.evaluate(cmp), value.evaluate(cmp));
	}
}

unsigned int metafuck::impl::array::get(Compiler& cmp, unsigned int target, const Variable& var, const Evaluatable& index) {
	if (index.getType() == Type::INTEGER) {
		return cmp.bf().getArrayPointer(cmp.getVar(var), static_cast<const Number&>(index).getValue());
	}
	else {
		auto i = cmp.bf().maketemp(index.evaluate(cmp));
		cmp.generated_ << cmp.bf().arrayGet(cmp.getVar(var), i, target);
		return target;
	}
}
