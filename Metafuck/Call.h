#ifndef CALL_H
#define CALL_H

#include "Argument.h"
#include <string>
#include <vector>

class Call
{
public:
	Call(std::string code);
	~Call();

private:
	std::string function_;
	std::vector<Argument> arguments_;
};

#endif;