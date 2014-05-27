#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <map>

enum Type{
    CALL, CALLLIST, STRING, VARIABLE, INTEGER,
    EVALUATABLE,  // = CALL, VARIABLE or INTEGER (something that has a result/a value)
    CALLABLE, // = CALL or CALLLIST (something that can be called)
    EXPRESSION
};

class Argument
{
public:
	virtual Type getType() const = 0;
	virtual ~Argument() = default;
};

Argument* parseArgument(std::string const& code);

inline bool isEvaluatable(const Type t) {
	return t == Type::CALL || t == Type::INTEGER || t == Type::VARIABLE || t == Type::EVALUATABLE;
}

inline bool isCallable(const Type t) {
	return t == Type::CALL || t == Type::CALLLIST || t == Type::CALLABLE;
}
#endif
