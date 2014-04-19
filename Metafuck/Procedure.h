#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Call.h"
#include <string>
class Procedure : Call
{
public:
	Procedure(std::string code);
	virtual bool returns() const;
};

#endif