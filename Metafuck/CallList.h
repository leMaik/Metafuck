#ifndef CALLLIST_H
#define CALLLIST_H

#include "Argument.h"
#include "Call.h"
#include <string>
#include <memory>
#include <vector>

class CallList : public Call
{
public:
	CallList() = default;
	CallList(std::string code);

	void compile(Compiler& cmp, Brainfuck& bf);
	unsigned int compileResult(Compiler& cmp, Brainfuck& bf);

	std::string toString() const;
	bool returns() const;
	Argument::Type getType() const;

private:
	std::vector<std::shared_ptr<Call>> calls_;
};

#endif
