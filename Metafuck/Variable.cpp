#include "Variable.h"
#include "compiler.h"

Variable::Variable(std::string name) : name_(name)
{
}

Variable::~Variable()
{
}

std::string Variable::getName() const {
	return name_;
}

unsigned int Variable::evaluate(Compiler& compiler) const {
    return compiler.getVar(*this);
}

void Variable::evaluate(Compiler& compiler, unsigned int target) const {
   compiler.generated_ << compiler.bf().copy(evaluate(compiler), target);
}

Type Variable::getType() const {
	return Type::VARIABLE;
}
