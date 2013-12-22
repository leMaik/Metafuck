#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "helper.h"
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
	CallList program(code_);

	//Phase 2: Statements -> std::vector<std::string>
	//std::string currentElement = "";
	//bool isString = false;
	//for (const std::string& statement : statements) {
	//	lexed_.push_back(std::vector<std::string>());
	//	for (const char& c : statement) {
	//		switch (c) {
	//		case ',':
	//		case '(':
	//		case ')':
	//			if (!isString) {
	//				lexed_[lexed_.size() - 1].push_back(currentElement);
	//				currentElement = "";
	//			}
	//			else {
	//				currentElement += c;
	//			}
	//			break;
	//		case '"':
	//			isString = !isString;
	//			currentElement += c;
	//			break;
	//		default:
	//			currentElement += c;
	//			break;
	//		}
	//	}
	//}
	//return statements.size();
	return 0;
}

bool Compiler::isNumber(const std::string &s) const {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	char * p;
	std::strtol(s.c_str(), &p, 10);
	return (*p == 0);
}

bool Compiler::isString(const std::string &s) const {
	return s.length() >= 2 && s[0] == '"' && s[s.length() - 1] == '"';
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
			if (isString(statement[1])){//print a constant string
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
		else if (statement[0] == "add") {
			if (isNumber(statement[2])) {
				generated_ << b.add(getVar(statement[1], b), std::stoi(statement[2]));
			}
			else if (!isString(statement[2])) {
				generated_ << b.addCellTo(getVar(statement[1], b), getVar(statement[2], b), getVar(statement[1], b));
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