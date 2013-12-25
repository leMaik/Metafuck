#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Number.h"
#include "helper.h"
#include <algorithm>
#include <locale>

std::size_t Compiler::lex() {
	//Phase 1: Statements ermitteln
	lexed_ = CallList(code_);
	return 0;
}

unsigned int Compiler::getVar(const Variable& variable, Brainfuck &b) {
	auto var = vars_.find(variable.getName());
	if (var == vars_.end()) {
		return vars_[variable.getName()] = b.allocCell(1);
	}
	return var->second;
}

unsigned int Compiler::evaluateTo(Argument& arg) {
	switch (arg.getType()){
	case Argument::Type::CALL:{
			Call&c = static_cast<Call&>(arg);
			if (c.getFunction() == "iseq"){
				unsigned int result = bf_.allocCell(1);
				generated_ << bf_.isEqual(evaluateTo(c.getArg(0)), evaluateTo(c.getArg(1)), result);
				return result;
			}
	    }
		break;
	case Argument::Type::VARIABLE:
		return getVar(static_cast<Variable&>(arg), bf_);
		break;
	case Argument::Type::INTEGER:
		unsigned int t = bf_.allocCell(1);
		generated_ << bf_.set(t, static_cast<Number&>(arg).getValue());
		return t;
	}
	return 0;
}

void Compiler::evaluate(Argument& arg) {
	switch (arg.getType()) {
	case Argument::Type::CALLLIST:
		for (Call statement : static_cast<CallList&>(arg).statements) {
			evaluate(statement);
		}
		break;
	case Argument::Type::CALL:
		Call& c = static_cast<Call&>(arg);
		if (c.getFunction() == "print"){
			if (c.getArg(0).getType() == Argument::Type::STRING) {
				generated_ << bf_.printString(static_cast<String&>(c.getArg(0)).getValue());
			}
			else if (c.getArg(0).getType() == Argument::Type::VARIABLE){
				generated_ << bf_.print(getVar(static_cast<Variable&>(c.getArg(0)), bf_));
			}
		}
		else if (c.getFunction() == "input"){
			generated_ << bf_.input(evaluateTo(c.getArg(0)));
		}
		else if (c.getFunction() == "set"){
			if (c.getArg(0).getType() == Argument::Type::VARIABLE) {
				generated_ << bf_.set(getVar(static_cast<Variable&>(c.getArg(0)), bf_), static_cast<Number&>(c.getArg(1)).getValue());
			}
		}
		else if (c.getFunction() == "if"){
			unsigned int temp0 = bf_.allocCell(1);
			unsigned int temp1 = bf_.allocCell(1);
			unsigned int x = evaluateTo(c.getArg(0));
			generated_ << bf_.copy(x, temp1);
			generated_ << bf_.set(temp0, 1);
			generated_ << bf_.move(temp1) << "[";
			evaluate(static_cast<CallList&>(c.getArg(1)));
			generated_ << bf_.move(temp0) << "-";
			generated_ << bf_.set(temp1, 0) << "]";
			generated_ << bf_.move(temp0) << "[";
			evaluate(static_cast<CallList&>(c.getArg(2)));
			generated_ << bf_.move(temp0) << "-]";
			bf_.freeCell(temp0);
			bf_.freeCell(temp1);
			bf_.freeCell(x);
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