#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <map>

class Argument
{
public:
	enum Type{
		CALL, CALLLIST, STRING, VARIABLE, INTEGER,
		EVALUATABLE,  // = CALL, VARIABLE or INTEGER (something that has a result/a value)
		CALLABLE, // = CALL or CALLLIST (something that can be called)
	};

	virtual Argument::Type getType() const = 0;
	virtual ~Argument() = default;
};

Argument* parseArgument(std::string const& code);

bool isEvaluatable(const Argument::Type t);
bool isCallable(const Argument::Type t);
#endif