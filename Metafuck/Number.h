#ifndef NUMBER_H
#define NUMBER_H

#include "Argument.h"

class Number : public Argument
{
private:
	unsigned int value_;

public:
	Number(const std::string code);
	~Number();

	unsigned int getValue() const;
	Argument::Type getType() const;
};

#endif