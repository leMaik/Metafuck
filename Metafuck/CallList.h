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
	CallList(const CallList&) = delete;
	CallList& operator=(const CallList&) = delete;
	CallList(std::string code);

	void compile(Compiler& cmp) const;
	void evaluate(Compiler& compiler, unsigned int target) const;

	std::string toString() const;
	bool returns() const;
	Type getType() const;

    static const Type type = Type::CALLLIST;
private:
	std::vector<std::unique_ptr<Call>> calls_;
};

#endif
