#include "compiler.h"

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
				break;
			default:
				currentElement += c;
				break;
			}
		}
	}
	return statements.size();
}

void Compiler::compile() {
	Brainfuck b;
	generated_.clear();
	for (const std::vector<std::string> statement : lexed_) {
		if (statement[0] == "set") {
			generated_ << b.set(b.allocCell(1), std::stoi(statement[2]));
		}
		else if (statement[0] == "print") {
			generated_ << b.printString(statement[1]);
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