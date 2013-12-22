#include "Argument.h"
#include <iostream>
#include <ctype.h>

bool isNumber(std::string s) {
	for (char c : s){
		if (!isdigit(c)){
			return false;
		}
	}
	return true;
}

Argument Argument::fromString(std::string code)
{
	if (code[0] == '"' && code[code.length()-1] == '"'){
		std::cout << "It's a string!" << std::endl;
	}
	else if (isNumber(code)){
		std::cout << "It's a number!" << std::endl;
	}
	return Argument();
}