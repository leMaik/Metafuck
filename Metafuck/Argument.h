#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>

class Argument
{
public:
	enum Type{ CALL, CALLLIST, STRING, VARIABLE, INTEGER };

	virtual Argument::Type getType() const = 0;
	virtual ~Argument() = default;
};

Argument* parseArgument(std::string const& code);
#endif