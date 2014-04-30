#include "CallList.h"
#include "Call.h"
#include "helper.h"
#include "compiler.h"
#include "Expression.h"
#include "ExpressionString.h"
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <memory>

CallList::CallList(std::string code)
: statements_{}
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
						std::cout << statement.str() << std::endl;
						statements_.emplace_back(new Call(statement.str()));

						isStatement = false;
					}
					else {
						std::cout << "Expression: '" << statement.str() << "'" << std::endl;
						statements_.emplace_back(new ExpressionString(statement.str()));
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

std::string CallList::compile(Compiler& cmp, Brainfuck& bf){
	std::stringstream output;
	for (auto& statement : statements_) {
		if (statement->getType() == Argument::CALL){
			Call* call = static_cast<Call*>(statement.get());
			std::shared_ptr<Statement> ptr { (cmp.getStatement(*call)) }; //TODO use CallSignature
			if (!ptr){
				std::cout << "Unknown function '" << call->getFunction() << "'." << std::endl;
			}
			else {
				output << ptr->compile(cmp, bf);
			}
		}
		else if (statement->getType() == Argument::EXPRESSION){
			Expression(static_cast<ExpressionString*>(statement.get())->expression).compile(cmp, bf);
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
