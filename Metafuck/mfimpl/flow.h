#ifndef MFIMPL_FLOW_H
#define MFIMPL_FLOW_H

#include "../Call.h"

namespace metafuck {
	namespace impl {
		namespace flow {
			void if_fn(Compiler &compiler, const Evaluatable& var, const Call& doif);
			void if_else_fn(Compiler &compiler, const Evaluatable& var, const Call& doif, const Call& doelse);
			void while_fn(const Call& c);
			void do_while_fn(const Call& c);
			void for_fn(const Call& c);
		}
	}
}

#endif
