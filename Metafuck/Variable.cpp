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
   compiler.generated_ << compiler.bf().copy(evaluate(compiler), target);
}

Type Variable::getType() const {
	return Type::VARIABLE;
}
