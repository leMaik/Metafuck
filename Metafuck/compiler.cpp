#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Number.h"
#include "helper.h"
#include <algorithm>
#include <locale>

std::size_t Compiler::lex() {
	lexed_ = CallList(code_);
	return 0;
}

unsigned int Compiler::getVar(const Variable& variable) {
	auto var = vars_.find(variable.getName());
	if (var == vars_.end()) {
		return vars_[variable.getName()] = bf_.allocCell(1);
	}
	return var->second;
}

unsigned int Compiler::evaluateTo(Argument& arg) {
	switch (arg.getType()){
	case Argument::Type::CALL:
		{
			Call& c = static_cast<Call&>(arg);
			auto& function = predef_functions.find(c.getSignature());
			if (function != predef_functions.end())
				return function->second(c);
			else
				std::cerr << "Unknown function: " << c << std::endl;
		}
		break;
	case Argument::Type::VARIABLE:
		return getVar(static_cast<Variable&>(arg));
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
		auto& method = predef_methods.find(c.getSignature());
		if (method != predef_methods.end())
			method->second(c);
		else
			std::cerr << "Unknown method: " << c << std::endl;
	}
}

void Compiler::set(const Call& c) {
	generated_ << bf_.set(getVar(static_cast<Variable&>(c.getArg(0))), static_cast<Number&>(c.getArg(1)).getValue());
}

void Compiler::add_const(const Call& c) {
	generated_ << bf_.add(getVar(static_cast<Variable&>(c.getArg(0))), static_cast<Number&>(c.getArg(1)).getValue());
}

void Compiler::print(const Call& c) {
	switch (c.getArg(0).getType()){
	case Argument::STRING:
		generated_ << bf_.printString(static_cast<String&>(c.getArg(0)).getValue());
		break;
	case Argument::VARIABLE:
		generated_ << bf_.print(getVar(static_cast<Variable&>(c.getArg(0))));
		break;
	}
}

void Compiler::input(const Call& c) {
	generated_ << bf_.input(evaluateTo(c.getArg(0)));
}

void Compiler::if_fn(const Call& c) {
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

void Compiler::while_fn(const Call& c) {
	unsigned int temp = evaluateTo(c.getArg(0));
	generated_ << bf_.move(temp) << "[";
	bf_.freeCell(temp);
	evaluate(c.getArg(1));
	unsigned int temp2 = evaluateTo(c.getArg(0));
	generated_ << bf_.move(temp2) << "]";
	if (c.getArg(1).getType() == Argument::CALL) {
		bf_.freeCell(temp2);
	}
}

int Compiler::iseq(const Call& c) {
	unsigned int result = bf_.allocCell(1);
	generated_ << bf_.isEqual(evaluateTo(c.getArg(0)), evaluateTo(c.getArg(1)), result);
	return result;
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

CompilerEasyRegister Compiler::reg() {
	return CompilerEasyRegister(*this);
}

void Compiler::reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (const Call&)){
	predef_methods[CallSignature(callname, args)] = std::bind(fptr, this, std::placeholders::_1);// new TSpecificFunctor<void, Compiler, const Call&>(this, fptr);
}

void Compiler::reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, int (Compiler::*fptr) (const Call&)){
	predef_functions[CallSignature(callname, args)] = std::bind(fptr, this, std::placeholders::_1);
}

Compiler::Compiler(std::string c) : code_(c) {
	reg()
		("set", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::set)
		("add", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::add_const)
		("print", { Argument::STRING }, &Compiler::print)
		("print", { Argument::VARIABLE }, &Compiler::print)
		("getchar", { Argument::VARIABLE }, &Compiler::input)
		("if", { Argument::CALL, Argument::CALLLIST, Argument::CALLLIST }, &Compiler::if_fn)
		("if", { Argument::INTEGER, Argument::CALLLIST, Argument::CALLLIST }, &Compiler::if_fn)
		("iseq", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::iseq)
		("iseq", { Argument::INTEGER, Argument::VARIABLE }, &Compiler::iseq)
		("iseq", { Argument::VARIABLE, Argument::VARIABLE }, &Compiler::iseq)
		("while", { Argument::CALL, Argument::CALLLIST }, &Compiler::while_fn);
}

CompilerEasyRegister::CompilerEasyRegister(Compiler& owner) : owner_(owner) { }

CompilerEasyRegister& CompilerEasyRegister::operator () (std::string callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (const Call&)) {
	owner_.reg(callname, args, fptr);
	return *this;
}

CompilerEasyRegister& CompilerEasyRegister::operator () (std::string callname, const std::initializer_list<Argument::Type>& args, int (Compiler::*fptr) (const Call&)) {
	owner_.reg(callname, args, fptr);
	return *this;
}