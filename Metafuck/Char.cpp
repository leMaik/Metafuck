#include "Char.h"
#include "helper.h"
#include <iostream>

Char::Char(const std::string code)
{
	std::string c = code.substr(1, code.length() - 2);
	ReplaceStringInPlace(c, "\\n", "\n");
	ReplaceStringInPlace(c, "\\t", "\t");
	ReplaceStringInPlace(c, "\\\"", "\"");
	ReplaceStringInPlace(c, "\\/", "/");
	ReplaceStringInPlace(c, "\\\\", "\\");
	ReplaceStringInPlace(c, "\\\'", "\'");
	if (c.length() > 1)
		std::cout << "Char is longer than a char, will take first character." << std::endl;
	value_ = c.at(0);
}

Char::~Char()
{
}
