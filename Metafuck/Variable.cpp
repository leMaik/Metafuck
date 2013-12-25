#include "Variable.h"


Variable::Variable(std::string name) : name_(name)
{
}


Variable::~Variable()
{
}

std::string Variable::getName() const {
	return name_;
}

Argument::Type Variable::getType() const {
	return Argument::Type::VARIABLE;
}