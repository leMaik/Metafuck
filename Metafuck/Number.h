#ifndef NUMBER_H
#define NUMBER_H

#include "Argument.h"
#include "compiler.h"

class Number : public Evaluatable
{
protected:
	unsigned int value_;
	Number();

public:
	Number(const std::string code);
	~Number();

	unsigned int getValue() const;
	Type getType() const;

    unsigned int evaluate(Compiler& compiler) const;
    void evaluate(Compiler& compiler, unsigned int target) const;

	static const Type type = Type::INTEGER;
};

#endif
