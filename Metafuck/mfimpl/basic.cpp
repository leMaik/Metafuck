#include "basic.h"
#include "../compiler.h"

void metafuck::impl::basic::set(Compiler& compiler, const Variable& var, const Evaluatable& value) {
    value.evaluate(compiler, compiler.getVar(var, true));
}
