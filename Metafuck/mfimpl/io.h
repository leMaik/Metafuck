#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../compiler.h"
#include "../Call.h"

namespace metafuck {
	namespace impl {
		namespace io {
			void print(const Call& c, Compiler& compiler, Brainfuck& bf);
		}
	}
}

#endif