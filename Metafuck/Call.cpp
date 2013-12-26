#include "Call.h"
#include "helper.h"
#include <iostream>
#include <stack>

Call::Call(std::string code)
{
	std::size_t p = code.find('(');
	function_ = code.substr(0, p);
	code = code.substr(p + 1, code.length() - p - 2);

	std::string currentArgument = "";
	bool isString = false;
	std::stack<char> keller;

	for (size_t i = 0; i < code.length(); i++) {
		char c = code[i];
		if (!isString){
			switch (c) {
			case '(':
			case '{':
				keller.push(c);
				currentArgument += c;
				break;
			case ')':
				if (keller.empty() || pop(keller) != '(')
					throw;
				currentArgument += c;
				break;
			case '}':
				if (keller.empty() || pop(keller) != '{')
					throw;
				currentArgument += c;
				break;
			case ';':
				if (!keller.empty()) {
					currentArgument += c;
				}
				break;
			case '"':
				isString = true;
				currentArgument += c;
				break;
			case ' ':
				break;
			case ',':
				if (keller.size() > 0){ //if it isn't a comma that separates parameters of this call
					currentArgument += c;
				}
				break;
			default:
				currentArgument += c;
				break;
			}
		}
		else {
			if (c == '"') {
				isString = false;
			}
			currentArgument += c;
		}

		if (keller.empty() && !isString && (i == code.length() - 1 || code[i + 1] == ',')) {
			std::shared_ptr<Argument> arg(parseArgument(currentArgument));
			arguments_.push_back(arg);
			currentArgument = "";
		}
	}

	if (!keller.empty()) {
		throw;
	}
}

Argument::Type Call::getType() const {
	return CALL;
}

std::string Call::getFunction() const {
	return function_;
}

Argument& Call::getArg(unsigned int index) const {
	return *arguments_.at(index);
}

Call::~Call()
{
}
