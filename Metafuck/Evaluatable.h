#ifndef EVALUATABLE_H
#define EVALUATABLE_H

#include "Argument.h"

class Compiler;

class Evaluatable : public Argument
{
    public:
        virtual ~Evaluatable() = default;
        virtual unsigned int evaluate(Compiler& compiler) const = 0;

        static const Type type = Type::EVALUATABLE;
    protected:
    private:
};

#endif // EVALUATABLE_H
