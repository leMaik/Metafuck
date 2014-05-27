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

Type Variable::getType() const {
	return Type::VARIABLE;
}
