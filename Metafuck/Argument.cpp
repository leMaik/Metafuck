#include "Argument.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Variable.h"
#include "Number.h"
#include <iostream>
#include <ctype.h>

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

Argument* parseArgument(std::string const& code)
{
	if (isString(code)) {
		return new String(code.substr(1, code.length() - 2));
	}
	else if (isCallList(code)) {
		return new CallList(code.substr(1, code.length() - 2));
	}
	else if (isNumber(code)) {
		return new Number(code);
	}
	else if (isVar(code)) {
		return new Variable(code);
	}
	else if (code[code.length() - 1] == ')') {
		return new Call(code);
	}
	//return nullptr;
	throw; //TODO: Create an exception class for that
}

bool isEvaluatable(const Argument::Type t)
{
	return t == Argument::Type::CALL || t == Argument::Type::INTEGER || t == Argument::Type::VARIABLE;
}

bool isCallable(const Argument::Type t)
{
	return t == Argument::Type::CALL || t == Argument::Type::CALLLIST;
}