#ifndef CALLLIST_H
#define CALLLIST_H

#include "Argument.h"
#include "Statement.h"
#include <string>
#include <memory>
#include <vector>

class CallList : public Statement
{
public:
	CallList();
	CallList(std::string code);

	std::string compile(Compiler& cmp, Brainfuck& bf);
	std::string toString() const;
	bool returns() const;
	Argument::Type getType() const;

private:
	std::vector<std::string> statements_;
};

#endif
