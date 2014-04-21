#include "CallList.h"
#include "Call.h"
#include "helper.h"
#include "compiler.h"
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
				std::cout << statement.str() << std::endl;
				statements_.push_back(Call(statement.str()));

				//this clears the stringstream
				statement.str(std::string());
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

std::string CallList::compile(Compiler& cmp, Brainfuck& bf){
	std::stringstream output;
	for (auto& statement : statements_) {
		auto ptr = std::shared_ptr<Statement>(cmp.getStatement(statement)); //TODO use CallSignature
		if (ptr == nullptr){
			std::cout << "Unknown function '" << statement.getFunction() << "'." << std::endl;
		}
		else {
			output << ptr->compile(cmp, bf);
		}
	}
	return output.str();
}

std::string CallList::toString() const{
	return "CallList";
}

bool CallList::returns() const {
	return false;
}

Argument::Type CallList::getType() const {
	return CALLLIST;
}
