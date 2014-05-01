#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../Procedure.h"
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