#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Statement.h"
#include "Call.h"
#include <string>
#include <vector>
#include <memory>

class Procedure : public Statement
{
public:
	Procedure(Call const& call);
	bool returns() const;
	std::string toString() const;

	inline Argument& arg(unsigned int index) const {
		return call_.arg(index);
	}

	template<class T>
	inline T& arg(unsigned int index) const {
		return call_.arg<T>(index);
	}

private:
	Call const& call_;
};

#endif