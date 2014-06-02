#ifndef MFIMPL_BASIC_H
#define MFIMPL_BASIC_H

#include "../compiler.h"

namespace metafuck{
namespace impl {
	namespace basic {
		void set(Compiler& compiler, const Variable& var, const Evaluatable& value);
		void let(Compiler& compiler, const Variable& var, const Evaluatable& value);
}}}

#endif
