#include "String.h"


String::String(std::string s) : content(s)
{
}


String::~String()
{
}

Argument::Type String::getType() const {
	return STRING;
}