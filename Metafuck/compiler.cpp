#include "compiler.h"
#include <algorithm>
#include <locale>

bool Compiler::validate() {
	std::stack<char> keller;
	for (const char& c : code_) {
		switch (c) {
		case '(':
		case '{':
			keller.push(c);
			break;
		case ')':
			if (keller.empty() || pop(keller) != '(')
				return false;
			break;
		case '}':
			if (keller.empty() || pop(keller) != '{')
				return false;
			break;
		}
	}
	return keller.empty();
}

std::size_t Compiler::lex() {
	//Phase 1: Statements ermitteln
	std::vector<std::string> statements;
	std::string currentStatement;
	bool ignoreSpaces = true;
	for (const char& c : code_) {
		switch (c) {
		case ';':
			statements.push_back(currentStatement);
			currentStatement = "";
			break;
		case '"':
			ignoreSpaces = !ignoreSpaces;
			currentStatement += c;
			break;
		default:
			if (!(ignoreSpaces && c == ' ') && c != '\t' && c != '\r' && c != '\n') {
				currentStatement += c;
			}
			break;
		}
	}

	//Phase 2: Statements -> std::vector<std::string>
	std::string currentElement = "";
	bool isString = false;
	for (const std::string& statement : statements) {
		lexed_.push_back(std::vector<std::string>());
		for (const char& c : statement) {
			switch (c) {
			case ',':
			case '(':
			case ')':
				if (!isString) {
					lexed_[lexed_.size() - 1].push_back(currentElement);
					currentElement = "";
				}
				else {
					currentElement += c;
				}
				break;
			case '"':
				isString = !isString;
				currentElement += c;
				break;
			default:
				currentElement += c;
				break;
			}
		}
	}
	return statements.size();
}

bool Compiler::isNumber(const std::string& s) const {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	char * p;
	std::strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

unsigned int Compiler::getVar(const std::string &name, Brainfuck &b) {
	auto var = vars_.find(name);
	if (var == vars_.end()){
		return vars_[name] = b.allocCell(1);
	}
	return var->second;
}

void Compiler::compile() {
	Brainfuck b;
	generated_.clear();
	for (const std::vector<std::string> statement : lexed_) {
		if (statement[0] == "set") {
			if (isNumber(statement[2])){
				generated_ << b.set(getVar(statement[1], b), std::stoi(statement[2]));
			}
			else {
				generated_ << b.copy(getVar(statement[2], b), getVar(statement[1], b));
			}
		}
		else if (statement[0] == "print") {
			if (statement[1].at(0) == '"'){//print a constant string
				generated_ << b.printString(statement[1].substr(1, statement[1].length() - 2));
			}
			else if (isNumber(statement[1])) {//print a constant number
				unsigned int tmp = b.allocCell(1);
				b.set(tmp, std::stoi(statement[1]));
				generated_ << b.print(tmp);
				b.freeCell(tmp);
			}
			else {//print a variable (as ASCII char at the moment)
				generated_ << b.print(getVar(statement[1], b));//TODO: Add a printint-command that really prints a number as a number (ie. "65" instead of 'A').
			}
		}
	}
}

std::string Compiler::getCode() const {
	return code_;
}

std::string Compiler::getGeneratedCode() const {
	return generated_.str();
}

Compiler::Compiler(std::string c) : code_(c) { }