#ifndef CALL_H
#define CALL_H

#include "Argument.h"
#include <string>
#include <vector>
#include <memory>

class Call : public Argument
{
public:
	Call(std::string code);
	~Call();
	Argument::Type getType() const;

	std::string getFunction() const;
	Argument& getArg(unsigned int index) const;

private:
	std::string function_;
	std::vector<std::shared_ptr<Argument>> arguments_;
};

#endif;