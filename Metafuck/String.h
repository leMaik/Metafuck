#ifndef STRING_H
#define STRING_H

#include "Argument.h"
#include <string>

class String : public Argument
{
public:
	String(std::string s);
	~String();
	std::string content;

	Argument::Type getType() const;
};

#endif