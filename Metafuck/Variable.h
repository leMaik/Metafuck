#ifndef VARIABLE_H
#define VARIABLE_H

#include "Argument.h"
#include "Evaluatable.h"
#include <string>

class Variable : public Evaluatable
{
private:
	std::string name_;

public:
	Variable(std::string name);
	~Variable();

	unsigned int evaluate(Compiler& compiler) const;
	void evaluate(Compiler& compiler, unsigned int target) const;

	std::string getName() const;
	Type getType() const;

	static const Type type = Type::VARIABLE;
};

#endif
