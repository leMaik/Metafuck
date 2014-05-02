#ifndef CALL_H
#define CALL_H

#include "Argument.h"
#include "brainfuck.h"
#include <string>
#include <vector>
#include <memory>

typedef std::pair<std::string, std::vector<Argument::Type>> CallSignature;
class Compiler;

class Call : public Argument
{
public:
	Call(std::string code);
	Argument::Type getType() const;
	std::string getFunction() const;

	virtual void compile(Compiler& cmp, Brainfuck& bf);
	virtual unsigned int compileResult(Compiler& cmp, Brainfuck& bf);

	CallSignature signature() const;
	bool matches(CallSignature sig) const;

	inline Argument& arg(unsigned int index) const {
		return *arguments_.at(index);
	}

	template<class T>
	inline T& arg(unsigned int index) const {
		return *static_cast<T*>(arguments_[index].get());
	}

protected:
	Call() = default;

private:
	std::string function_;
	std::vector<std::shared_ptr<Argument>> arguments_;
};

std::ostream& operator<<(std::ostream &strm, const Call &c);

#endif
