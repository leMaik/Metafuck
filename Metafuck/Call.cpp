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
	bool isEscaped = false;
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
			if (!isEscaped){
				if (c == '"') {
					isString = false;
				}
				else if (c == '\\'){
					isEscaped = true;
				}
			}
			else {
				isEscaped = false;
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

std::ostream& operator<<(std::ostream &strm, const Call &c) {
	strm << c.getFunction() << "(";
	bool first = true;
	for (auto& a : c.getSignature().second){
		if (!first)
			strm << ", ";
		else
			first = false;

		switch (a) {
		case Argument::CALL:
			strm << "Call";
			break;
		case Argument::CALLLIST:
			strm << "CallList";
			break;
		case Argument::STRING:
			strm << "String";
			break;
		case Argument::VARIABLE:
			strm << "Variable";
			break;
		case Argument::INTEGER:
			strm << "Integer";
			break;
		case Argument::EVALUATABLE:
			strm << "Evaluatable";
			break;
		case Argument::CALLABLE:
		   strm << "Callable";
		   break;
		}
	}
	return strm << ")";
}

Argument::Type Call::getType() const {
	return CALL;
}

CallSignature Call::getSignature() const {
	std::vector<Argument::Type> params;
	for (auto &a : arguments_) {
		params.push_back(a->getType());
	}
	CallSignature r;
	r.first = getFunction();
	r.second = params;
	return r;
}

bool Call::matches(CallSignature sig) const {
	if (arguments_.size() != sig.second.size() || sig.first != function_) //TODO: Make case-insensitive check here?
		return false;
	//assert: lengths are now equal
	std::vector<Type>::iterator itr = sig.second.begin();
	for (auto& i : arguments_) {
		if (i->getType() != *itr
			&& !(*itr == Type::EVALUATABLE && isEvaluatable(i->getType()))
			&& !(*itr == Type::CALLABLE && isCallable(i->getType()))
			)
			return false;
		itr++;
	}
	return true;
}

std::string Call::getFunction() const {
	return function_;
}

Call::~Call()
{
}
