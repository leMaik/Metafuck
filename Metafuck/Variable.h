#ifndef VARIABLE_H
#define VARIABLE_H

#include "Argument.h"
#include <string>

class Variable : public Argument
{
private:
	std::string name_;

public:
	Variable(std::string name);
	~Variable();

	std::string getName() const;
	Argument::Type getType() const;

	static const Type type = Type::VARIABLE;
};

#endif
