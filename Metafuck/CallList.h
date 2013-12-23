#ifndef CALLLIST_H
#define CALLLIST_H

#include "Argument.h"
#include "Call.h"
#include <string>
#include <stack>

class CallList : public Argument
{
public:
	CallList();
	CallList(std::string code);
	~CallList();
	Argument::Type getType() const;

	std::vector<Call> statements;
};

#endif
