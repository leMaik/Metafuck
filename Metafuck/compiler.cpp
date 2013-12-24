#include "compiler.h"
#include "CallList.h"
#include "Call.h"
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
			Call c = (Call)arg;
	}
	//TODO: Evaluate other types
}

void Compiler::compile() {
	Brainfuck b;
	generated_.clear();
	evaluate(lexed_);
}

std::string Compiler::getCode() const {
	return code_;
}

std::string Compiler::getGeneratedCode() const {
	return generated_.str();
}

Compiler::Compiler(std::string c) : code_(c) { }