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

inline bool isNumber(std::string const& s) {
	for (char const &c : s) {
		if (!isdigit(c))
			return false;
	}
	return true;
}

inline bool isVar(std::string const& s) {
	for (char c : s){
		if (!isalnum(c))
			return false;
	}
	return true;
}

inline bool isCallList(std::string const& s) {
	return s.length() >= 2 && *s.begin() == '{' && *s.rbegin() == '}';
}

inline bool isString(std::string const& s) {
	return s.length() >= 2 && *s.begin() == '"' && *s.rbegin() == '"';
}

inline bool isChar(std::string const& s) {
	return s.length() >= 3 && *s.begin() == '\'' && *s.rbegin() == '\'';
}
#endif
