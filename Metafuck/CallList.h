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

	void compile(Compiler& cmp) const;
	unsigned int evaluate(Compiler& compiler) const;

	std::string toString() const;
	bool returns() const;
	Type getType() const;

    static const Type type = Type::CALLLIST;
private:
	std::vector<std::shared_ptr<Call>> calls_;
};

#endif
