#ifndef FLOW_H
#define FLOW_H

#include "../Call.h"

namespace metafuck {
	namespace impl {
		namespace flow {
			void if_fn(Compiler &compiler, const Evaluatable& var, const Call& doif);
			void if_else_fn(const Call& c);
			void while_fn(const Call& c);
			void do_while_fn(const Call& c);
			void for_fn(const Call& c);
		}
	}
}

#endif
