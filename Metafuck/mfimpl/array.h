#ifndef MFIMPL_ARRAY_H
#define MFIMPL_ARRAY_H

#include "../compiler.h"
#include "../Variable.h"
#include "../Evaluatable.h"
#include "../Number.h"

namespace metafuck {
namespace impl {
namespace array {
			void init(Compiler& cmp, const Variable& var, const Number& length);
			void set(Compiler& cmp, const Variable& var, const Evaluatable& index, const Evaluatable& value);
			unsigned int get(Compiler& cmp, unsigned int target, const Variable& var, const Evaluatable& index);
}}}
#endif