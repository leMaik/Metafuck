#ifndef FLOW_H
#define FLOW_H

#include "../Call.h"

namespace metafuck {
	namespace impl {
		namespace flow {
			void if_fn(const Call& c);
			void if_else_fn(const Call& c);
			void while_fn(const Call& c);
			void do_while_fn(const Call& c);
			void for_fn(const Call& c);
		}
	}
}

#endif