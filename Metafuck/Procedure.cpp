#include "Procedure.h"
#include <string>


Procedure::Procedure(std::string code) : Call(code)
{
}

bool Procedure::returns() const {
	return false;
}

std::string Procedure::toString() const {
	return "procedure";
}