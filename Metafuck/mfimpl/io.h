#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../compiler.h"
#include "../Call.h"
#include "../String.h"
#include "../Evaluatable.h"

namespace metafuck {
	namespace impl {
		namespace io {
			void print_str(Compiler &compiler, const String& str);
			void print_var(Compiler &compiler, const Evaluatable& var);
			void getchar(Compiler &compiler, const Variable& var);
		}
	}
}

#endif
