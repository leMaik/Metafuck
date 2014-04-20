#include "Call.h"
#include "helper.h"
#include <iostream>
#include <stack>

Call::Call(std::string code) : Statement(code)
{
	std::size_t p = code.find('(');
	function_ = code.substr(0, p);
	code = code.substr(p + 1, code.length() - p - 2);

	std::string currentArgument = "";
	bool isString = false;
	bool isChar = false;
	bool isEscaped = false;
	std::stack<char> keller;

	for (size_t i = 0; i < code.length(); i++) {
		char c = code[i];
		if (!isString && !isChar){
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
			case '\'':
				isChar = true;
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
			if (!isEscaped){
				if (c == '"') {
					isString = false;
				}
				else if (c == '\'') {
					isChar = false;
				}
				else if (c == '\\') {
					isEscaped = true;
				}
			}
			else {
				isEscaped = false;
			}
			currentArgument += c;
		}

		if (keller.empty() && !isString && !isChar && (i == code.length() - 1 || code[i + 1] == ',')) {
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

//CallSignature Call::getSignature() const {
//	std::vector<Argument::Type> params;
//	for (auto &a : arguments_) {
//		params.push_back(a->getType());
//	}
//	CallSignature r;
//	r.first = getFunction();
//	r.second = params;
//	return r;
//}

//bool Call::matches(CallSignature sig) const {
//	if (arguments_.size() != sig.second.size() || sig.first != function_) //TODO: Make case-insensitive check here?
//		return false;
//	//assert: lengths are now equal
//	std::vector<Type>::iterator itr = sig.second.begin();
//	for (auto& i : arguments_) {
//		if (i->getType() != *itr
//			&& !(*itr == Type::EVALUATABLE && isEvaluatable(i->getType()))
//			&& !(*itr == Type::CALLABLE && isCallable(i->getType()))
//			)
//			return false;
//		itr++;
//	}
//	return true;
//}

std::string Call::getFunction() const {
	return function_;
}
