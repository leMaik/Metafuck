#include "CallList.h"
#include "Call.h"
#include "helper.h"
#include <iostream>
#include <vector>
#include <stack>

CallList::CallList(std::string code)
{
	std::string currentCall = "";
	bool isString = false;
	bool isStatement = false;

	std::stack<char> keller;
	for (const char& c : code) {
		if (!isString){
			switch (c) {
			case '(':
				isStatement = true;
			case '{':
				keller.push(c);
				currentCall += c;
				break;
			case ')':
				if (keller.empty() || pop(keller) != '(')
					throw;
				currentCall += c;
				break;
			case '}':
				if (keller.empty() || pop(keller) != '{')
					throw;
				currentCall += c;
				break;
			case ';':
				if (!keller.empty()) {
					currentCall += c;
				}
				break;
			case '"':
				isString = true;
				currentCall += c;
				break;
			case ' ':
				break;
			default:
				currentCall += c;
				break;
			}
			if (keller.empty() && isStatement){
				statements.push_back(Call(currentCall));
				std::cout << "Found call: " << currentCall << std::endl;
				currentCall = "";
				isStatement = false;
			}
		}
		else {
			if (c == '"') {
				isString = false;
			}
			currentCall += c;
		}
	}

	if (!keller.empty()) {
		throw;
	}
}

CallList::CallList()
{
}

CallList::~CallList()
{
}

Argument::Type CallList::getType() const {
	return CALLLIST;
}