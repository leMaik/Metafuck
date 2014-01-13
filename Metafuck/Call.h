#ifndef CALL_H
#define CALL_H

#include "Argument.h"
#include <string>
#include <vector>
#include <memory>

typedef std::pair<std::string, std::vector<Argument::Type>> CallSignature;

class Call : public Argument
{
public:
	Call(std::string code);
	~Call();
	Argument::Type getType() const;
	CallSignature getSignature() const;
	bool matches(CallSignature sig) const;

	std::string getFunction() const;
	Argument& getArg(unsigned int index) const;

private:
	std::string function_;
	std::vector<std::shared_ptr<Argument>> arguments_;
};

std::ostream& operator<<(std::ostream &strm, const Call &c);

#endif
