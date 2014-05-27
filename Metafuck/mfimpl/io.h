#ifndef MFIMPL_IO_H
#define MFIMPL_IO_H

#include "../compiler.h"
#include "../Call.h"
#include "../String.h"

namespace metafuck {
	namespace impl {
		namespace io {
			void print_str(Compiler& compiler, String& str);
			void print_var(Compiler& compiler, Variable& var);
			void getchar(Compiler& compiler, Variable& var);
		}
	}
}

#endif