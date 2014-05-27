#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include "../compiler.h"
#include "../Call.h"
#include "../String.h"

namespace metafuck {
	namespace impl {
		namespace logic {
            unsigned int iseq(Compiler &compiler, unsigned int result, const Variable& var, const Variable& varb);
		}
	}
}

#endif // LOGIC_H_INCLUDED
