#include "String.h"
#include "helper.h"

String::String(std::string s) : value_(s)
{
	ReplaceStringInPlace(value_, "\\r", "\r");
	ReplaceStringInPlace(value_, "\\n", "\n");
	ReplaceStringInPlace(value_, "\\t", "\t");
	ReplaceStringInPlace(value_, "\\\"", "\"");
	ReplaceStringInPlace(value_, "\\/", "/");
	ReplaceStringInPlace(value_, "\\\\", "\\");
	ReplaceStringInPlace(value_, "\\\'", "\'");
}

String::~String()
{
}

std::string String::getValue() const {
	return value_;
}

Type String::getType() const {
	return STRING;
}
