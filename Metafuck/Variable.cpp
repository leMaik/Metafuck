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
    unsigned int cell = compiler.bf().allocCell();
    evaluate(compiler, cell);
    return cell;

    return compiler.getVar(*this); //<-- TODO: use that, but we
                                   //need to return the information that it's not
                                   //temporary => something to do for bfcell later.
}

void Variable::evaluate(Compiler& compiler, unsigned int target) const {
	unsigned int var = compiler.getVar(*this);
	if (var != target)
		compiler.generated_ << compiler.bf().copy(var, target);

    //compiler.generated_ << compiler.bf().copy(evaluate(compiler), target);
	//TODO ^-- use that when we use the solution above	
}

Type Variable::getType() const {
	return Type::VARIABLE;
}
