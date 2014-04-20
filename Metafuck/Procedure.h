#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Call.h"
#include <string>
class Procedure : public Call
{
public:
	Procedure(std::string code);
	bool returns() const;
	std::string toString() const;
};

#endif