#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include "../compiler.h"
#include "../Call.h"
#include "../String.h"

namespace metafuck {
namespace impl {
namespace logic {
	unsigned int iseq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb);
	unsigned int isnoteq(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb);

	unsigned int and(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb);
	unsigned int or(Compiler &compiler, unsigned int result, const Evaluatable& var, const Evaluatable& varb);
	unsigned int not(Compiler &compiler, unsigned int result, const Evaluatable& var);
}}}

#endif // LOGIC_H_INCLUDED
