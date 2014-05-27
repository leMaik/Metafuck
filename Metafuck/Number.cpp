#include "Number.h"


Number::Number(const std::string code) : value_(std::stol(code))
{
}

Number::Number()
{
}

Number::~Number()
{
}

unsigned int Number::getValue() const {
	return value_;
}

Type Number::getType() const{
	return Type::INTEGER;
}

unsigned int Number::evaluate(Compiler& compiler) const {
    unsigned int cell = compiler.bf().allocCell();
    evaluate(compiler, cell);
    return cell;
}

void Number::evaluate(Compiler& compiler, unsigned int target) const {
    compiler.generated_ << compiler.bf().set(target, getValue());
}
