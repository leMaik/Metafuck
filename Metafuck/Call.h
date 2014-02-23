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
	inline Argument& arg(unsigned int index) const {
		return *arguments_.at(index);
	}

	template<class T>
	inline T& arg(unsigned int index) const {
		return static_cast<T&>(*arguments_[index]);
	}

private:
	std::string function_;
	std::vector<std::shared_ptr<Argument>> arguments_;
};

std::ostream& operator<<(std::ostream &strm, const Call &c);

#endif
