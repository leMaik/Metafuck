#include "String.h"


String::String(std::string s) : value_(s)
{
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