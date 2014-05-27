#ifndef CALL_H
#define CALL_H

#include "Argument.h"
#include "brainfuck.h"
#include "Evaluatable.h"
#include <string>
#include <vector>
#include <memory>

typedef std::pair<std::string, std::vector<Type>> CallSignature;
class Compiler;

class Call : public Evaluatable
{
public:
	Call(std::string code);
	Type getType() const;
	std::string getFunction() const;

	virtual void compile(Compiler& cmp) const;
	virtual unsigned int evaluate(Compiler& compiler) const;

	CallSignature signature() const;
	bool matches(CallSignature sig) const;

	inline Argument& arg(unsigned int index) const {
		return *arguments_.at(index);
	}

	template<class T>
	inline T& arg(unsigned int index) const {
		return *static_cast<T*>(arguments_[index].get());
	}

    static const Type type = Type::CALL;

protected:
	Call() = default;

private:
	std::string function_;
	std::vector<std::shared_ptr<Argument>> arguments_;
};

std::ostream& operator<<(std::ostream &strm, const Call &c);

#endif
