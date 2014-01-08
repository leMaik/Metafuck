#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <map>

class Argument
{
public:
	const enum Type{
		CALL, CALLLIST, STRING, VARIABLE, INTEGER,
		EVALUATABLE  // = CALL, VARIABLE or INTEGER
	};

	virtual Argument::Type getType() const = 0;
	virtual ~Argument() = default;
};

Argument* parseArgument(std::string const& code);
#endif