#ifndef CALLLIST_H
#define CALLLIST_H

#include "Argument.h"
#include "Call.h"
#include "Statement.h"
#include <string>
#include <stack>
#include <memory>

class CallList : public Argument
{
public:
	CallList();
	CallList(std::string code);
	~CallList();
	Argument::Type getType() const;

	std::vector<std::unique_ptr<Statement>> statements;
};

#endif
