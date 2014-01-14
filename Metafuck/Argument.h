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

inline bool isEvaluatable(const Argument::Type t) {
	return t == Argument::Type::CALL || t == Argument::Type::INTEGER || t == Argument::Type::VARIABLE;
}

inline bool isCallable(const Argument::Type t) {
	return t == Argument::Type::CALL || t == Argument::Type::CALLLIST;
}
#endif