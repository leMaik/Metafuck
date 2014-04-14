#include "CallList.h"
#include "Call.h"
#include "helper.h"
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

CallList::CallList(std::string code)
{
	std::stringstream statement;
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
				statement << c;
				break;
			case ')':
				if (keller.empty() || pop(keller) != '(')
					throw;
				statement << c;
				break;
			case '}':
				if (keller.empty() || pop(keller) != '{')
					throw;
				statement << c;
				break;
			case ';':
				if (!keller.empty()) {
					statement << c;
				}
				break;
			case '"':
				isString = true;
				statement << c;
				break;
			case '\'':
				isChar = true;
				statement << c;
				break;
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				break;
			default:
				statement << c;
				break;
			}
			if (keller.empty() && isStatement){
				statements.push_back(Call(statement.str()));
				statement.clear();
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
			statement << c;
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