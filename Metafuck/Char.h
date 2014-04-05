#ifndef CHAR_H
#define CHAR_H

#include "Number.h"

class Char : public Number
{
public:
	Char(const std::string code);
	~Char();
};

#endif

