#include "Argument.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Variable.h"
#include "Number.h"
#include "Char.h"
#include <iostream>
#include <ctype.h>

Argument* parseArgument(std::string const& code)
{
	if (isString(code)) {
		return new String(code.substr(1, code.length() - 2));
	}
	else if (isCallList(code)) {
		return new CallList(code.substr(1, code.length() - 2));
	}
	else if (isNumber(code)) {
		return new Number(code);
	}
	else if (isChar(code)){
		return new Char(code);
	}
	else if (isVar(code)) {
		return new Variable(code);
	}
	else if (code[code.length() - 1] == ')') {
		return new Call(code);
	}
	/*else {
		return new ExpressionString(code);
	}*/
	throw; //TODO: Create an exception class for that
}