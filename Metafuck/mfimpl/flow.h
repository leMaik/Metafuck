#ifndef MFIMPL_FLOW_H
#define MFIMPL_FLOW_H

#include "../Call.h"
#include "../Variable.h"

namespace metafuck {
	namespace impl {
		namespace flow {
			void if_fn(Compiler &compiler, const Evaluatable& var, const Call& doif);
			void if_else_fn(Compiler &compiler, const Evaluatable& var, const Call& doif, const Call& doelse);
			void while_fn(Compiler &compiler, const Evaluatable& var, const Call& inner);
			void do_while_fn(Compiler &compiler, const Call& inner, const Evaluatable& var);
			void for_fn(Compiler &compiler, const Variable& var, const Evaluatable& init, const Evaluatable& condition, const Call& inner, const Call& step);
		}
	}
}

#endif
