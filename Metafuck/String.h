#ifndef STRING_H
#define STRING_H

#include "Argument.h"
#include <string>

class String : public Argument
{
private:
	std::string value_;

public:
	String(std::string s);
	~String();

	std::string getValue() const;
	Type getType() const;

	static const Type type = Type::STRING;
};

#endif
