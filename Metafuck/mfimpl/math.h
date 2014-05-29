#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include "../compiler.h"
#include "../Variable.h"
#include "../Number.h"

namespace metafuck {
namespace impl {
namespace math {
	void add_const(Compiler& compiler, const Variable& var, const Number& c);
	void add_ev(Compiler& compiler, const Variable& var, const Evaluatable& c);

	void sub_const(Compiler& compiler, const Variable& var, const Number& c);
	void sub_ev(Compiler& compiler, const Variable& var, const Evaluatable& c);

	unsigned int div(Compiler& compiler, unsigned int target, const Evaluatable& divident, const Evaluatable& divisor);
	unsigned int mod(Compiler& compiler, unsigned int target, const Evaluatable& divident, const Evaluatable& divisor);
}}}

#endif

