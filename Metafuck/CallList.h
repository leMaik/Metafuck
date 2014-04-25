#ifndef CALLLIST_H
#define CALLLIST_H

#include "Argument.h"
#include "Statement.h"
#include "Call.h"
#include <string>
#include <memory>
#include <vector>

class CallList : public Call
{
public:
	CallList() = default;
	CallList(std::string code);

	std::string compile(Compiler& cmp, Brainfuck& bf);
	std::string toString() const;
	bool returns() const;
	Argument::Type getType() const;

private:
	std::vector<std::unique_ptr<Argument>> statements_;
};

#endif
