#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "helper.h"
#include <algorithm>
#include <locale>

std::size_t Compiler::lex() {
	//Phase 1: Statements ermitteln
	lexed_ = CallList(code_);
	return 0;
}

unsigned int Compiler::getVar(const std::string &name, Brainfuck &b) {
	auto var = vars_.find(name);
	if (var == vars_.end()){
		return vars_[name] = b.allocCell(1);
	}
	return var->second;
}

void Compiler::evaluate(Argument& arg) {
	switch (arg.getType()) {
		case Argument::Type::CALLLIST:
			for (Call statement : lexed_.statements) {
				evaluate(statement);
			}
			break;
		case Argument::Type::CALL:
			Call& c = static_cast<Call&>(arg);
			if (c.getFunction() == "print"){
				if (c.getArg(0).getType() == Argument::Type::STRING) {
					generated_ << bf_.printString(static_cast<String&>(c.getArg(0)).content);
				}
			}
	}
	//TODO: Evaluate other types
}

void Compiler::compile() {
	evaluate(lexed_);
}

std::string Compiler::getCode() const {
	return code_;
}

std::string Compiler::getGeneratedCode() const {
	return generated_.str();
}

Compiler::Compiler(std::string c) : code_(c) { }