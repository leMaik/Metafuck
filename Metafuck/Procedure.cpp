#include "Procedure.h"
#include <string>

Procedure::Procedure(Call const& call) : call_(call)
{
}

bool Procedure::returns() const {
	return false;
}

std::string Procedure::toString() const {
	return "procedure";
}