#include "CallList.h"
#include "helper.h"
#include "Expression.h"
#include "ExpressionString.h"
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <memory>

CallList::CallList(std::string code)
: calls_{}
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
				keller.push(c);
				statement << c;
				break;
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
				if (keller.empty()) {
					if (isStatement){
						if (isCallList(statement.str()))
							calls_.emplace_back(new CallList(statement.str().substr(1, statement.str().size() - 2)));
						else
							calls_.emplace_back(new Call(statement.str()));
						isStatement = false;
					}
					else {
						std::cout << "Expression: '" << statement.str() << "'" << std::endl;
						//TODO translate expression to calllist here
						//calls_.emplace_back(new ExpressionString(statement.str()));
					}

					//this clears the stringstream
					statement.str(std::string());
					statement.clear();
				}
				else {
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

void CallList::compile(Compiler& cmp) const {
	auto scope = cmp.scope();
	for (auto& statement : calls_) {
		statement->compile(cmp);
	}
}

void CallList::evaluate(Compiler& compiler, unsigned int target) const {
	compiler.warning(this, "Using return value of call list results in undefined behaviour");
	compile(compiler);
}

std::string CallList::toString() const{
	return "CallList";
}

bool CallList::returns() const {
	return false;
}

Type CallList::getType() const {
	return CALLLIST;
}
