#include "String.h"
#include "helper.h"


String::String(std::string s) : value_(s)
{
	ReplaceStringInPlace(value_, "\\n", "\n");
	ReplaceStringInPlace(value_, "\\t", "\t");

	ReplaceStringInPlace(value_, "\\\\", "\\");
}

String::~String()
{
}

std::string String::getValue() const {
	return value_;
}

Argument::Type String::getType() const {
	return STRING;
}