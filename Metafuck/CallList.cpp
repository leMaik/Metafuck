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
	bool isChar = false;
	bool isStatement = false;
	bool isEscaped = false;

	std::stack<char> keller;
	for (const char& c : code) {
		if (!isString && !isChar){
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
			case '\'':
				isChar = true;
				currentCall += c;
				break;
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				break;
			default:
				currentCall += c;
				break;
			}
			if (keller.empty() && isStatement){
				statements.push_back(Call(currentCall));
				currentCall = "";
				isStatement = false;
			}
		}
		else {
			if (!isEscaped) {
				if (isString && c == '"') {
					isString = false;
				}
				else if (isChar && c == '\'') {
					isChar = false;
				}
				else if (c == '\\') {
					isEscaped = true;
				}
			}
			else {
				isEscaped = false;
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