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
		return (vars_[variable.getName()] = bf_.allocCell());
	}
	return var->second;
}

unsigned int Compiler::evaluateTo(Argument& arg) {
	switch (arg.getType()){
	case Argument::CALL:
	{
						   Call& c = static_cast<Call&>(arg);
						   auto function = std::find_if(std::begin(predef_functions), std::end(predef_functions),
							   [&c](std::pair<CallSignature, std::function<int(const Call&, unsigned int)>> k) -> bool {
							   return c.matches(k.first);
						   });
						   if (function != predef_functions.end()){
							   return function->second(c, bf_.allocCell());
						   }
						   else {
							   std::cerr << "Unknown function: " << c << std::endl;
						   }
	}
		break;
	case Argument::Type::VARIABLE:
		return getVar(static_cast<Variable&>(arg));
	case Argument::Type::INTEGER:
	{
									unsigned int t = bf_.allocCell();
									generated_ << bf_.set(t, static_cast<Number&>(arg).getValue());
									return t;
	}
	default:
		return 0;
	}
	return 0;
}

void Compiler::evaluateTo(Argument& arg, unsigned int target) {
	switch (arg.getType()){
	case Argument::CALL:
	{
						   Call& c = static_cast<Call&>(arg);
						   auto function = std::find_if(std::begin(predef_functions), std::end(predef_functions),
							   [&c](std::pair<CallSignature, std::function<int(const Call&, unsigned int)>> k) -> bool {
							   return c.matches(k.first);
						   });
						   if (function != predef_functions.end()){
							   generated_ << function->second(c, target);
						   }
						   else {
							   std::cerr << "Unknown function: " << c << std::endl;
						   }
	}
		break;
	case Argument::Type::VARIABLE:
		//TODO: In this case, the produced variable is temporary and should be treated as such
		generated_ << bf_.copy(getVar(static_cast<Variable&>(arg)), target);
		break;
	case Argument::Type::INTEGER:
		generated_ << bf_.set(target, static_cast<Number&>(arg).getValue());
		break;
	default:
		//TODO: Exception
		break;
	}
}

void Compiler::evaluate(Argument& arg) {
	switch (arg.getType()) {
	case Argument::Type::CALLLIST:
		for (Call statement : static_cast<CallList&>(arg).statements) {
			evaluate(statement);
		}
		break;
	case Argument::Type::CALL:
	{
								 Call& c = static_cast<Call&>(arg);
								 auto method = std::find_if(std::begin(predef_methods), std::end(predef_methods),
									 [&c](std::pair<CallSignature, std::function<void(const Call&)>> k) -> bool {
									 return c.matches(k.first);
								 });
								 if (method != std::end(predef_methods)){
									 method->second(c);
								 }
								 else {
									 std::cerr << "Unknown method: " << c << std::endl;
								 }
	}
		break;
	default:
		std::cerr << "Argument is not evaluatable." << std::endl;
		break;
	}
}

void Compiler::set(const Call& c) {
	if (c.arg(1).getType() == Argument::INTEGER)
		generated_ << bf_.set(getVar(static_cast<Variable&>(c.arg(0))), static_cast<Number&>(c.arg(1)).getValue());
	else
		generated_ << bf_.copy(evaluateTo(c.arg(1)), getVar(c.arg<Variable>(0)));
}

void Compiler::add_const(const Call& c) {
	generated_ << bf_.add(getVar(c.arg<Variable>(0)), c.arg<Number>(1).getValue());
}

void Compiler::add_ev(const Call& c) {
	generated_ << bf_.addCellTo(getVar(c.arg<Variable>(0)), evaluateTo(c.arg(1)), getVar(c.arg<Variable>(0)));
}

void Compiler::sub_const(const Call& c) {
	generated_ << bf_.sub(getVar(c.arg<Variable>(0)), c.arg<Number>(1).getValue());
}

void Compiler::sub_ev(const Call& c) {
	generated_ << bf_.subCellFrom(getVar(c.arg<Variable>(0)), evaluateTo(c.arg(1)), getVar(c.arg<Variable>(0)));
}

void Compiler::div(const Call& c) {
	unsigned int cells = bf_.allocCell(2);
	generated_ << bf_.divmod(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), cells);
	generated_ << bf_.copy(cells, getVar(c.arg<Variable>(2)));
	bf_.freeCell(cells);
	bf_.freeCell(cells + 1);
}

void Compiler::mod(const Call& c) {
	unsigned int cells = bf_.allocCell(2);
	generated_ << bf_.divmod(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), cells);
	generated_ << bf_.copy(cells + 1, getVar(c.arg<Variable>(2)));
	bf_.freeCell(cells);
	bf_.freeCell(cells + 1);
}

void Compiler::print(const Call& c) {
	if (c.arg(0).getType() == Argument::STRING)
		generated_ << bf_.printString(c.arg<String>(0).getValue());
	else
		generated_ << bf_.print(evaluateTo(c.arg(0)));
}

void Compiler::printNumber(const Call& c) {
	generated_ << bf_.printNumber(evaluateTo(c.arg(0)));
}

void Compiler::input(const Call& c) {
	generated_ << bf_.input(evaluateTo(c.arg(0)));
}

void Compiler::if_fn(const Call& c) {
	unsigned int x = evaluateTo(c.arg(0));
	generated_ << bf_.move(x) << "[";
	evaluate(static_cast<CallList&>(c.arg(1)));
	generated_ << bf_.set(x, 0) << "]";
}

void Compiler::if_else_fn(const Call& c) {
	unsigned int temp0 = bf_.allocCell();
	unsigned int temp1 = bf_.allocCell();
	unsigned int x = evaluateTo(c.arg(0));
	generated_ << bf_.copy(x, temp1);
	generated_ << bf_.set(temp0, 1);
	generated_ << bf_.move(temp1) << "[";
	evaluate(static_cast<CallList&>(c.arg(1)));
	generated_ << bf_.move(temp0) << "-";
	generated_ << bf_.set(temp1, 0) << "]";
	generated_ << bf_.move(temp0) << "[";
	evaluate(static_cast<CallList&>(c.arg(2)));
	generated_ << bf_.move(temp0) << "-]";
	bf_.freeCell(temp0);
	bf_.freeCell(temp1);
	bf_.freeCell(x);
}

void Compiler::while_fn(const Call& c) {
	unsigned int temp = evaluateTo(c.arg(0));
	generated_ << bf_.move(temp) << "[";
	evaluate(c.arg(1));
	evaluateTo(c.arg(0), temp);
	generated_ << bf_.move(temp) << "]";
	if (c.arg(0).getType() == Argument::CALL) {
		bf_.freeCell(temp);
	}
}

void Compiler::do_while_fn(const Call& c) {
	unsigned int temp = bf_.allocCell();
	generated_ << bf_.set(temp, 1);
	generated_ << bf_.move(temp) << "[";
	evaluate(c.arg(0));
	evaluateTo(c.arg(1), temp);
	generated_ << bf_.move(temp) << "]";
	if (c.arg(1).getType() == Argument::CALL) {
		bf_.freeCell(temp);
	}
}

unsigned int Compiler::iseq(const Call& c, unsigned int result) {
	generated_ << bf_.isEqual(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), result);
	return result;
}

unsigned int Compiler::isnoteq(const Call& c, unsigned int result) {
	generated_ << bf_.isNotEqual(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), result);
	return result;
}

unsigned int Compiler::not_fn(const Call& c, unsigned int result) {
	generated_ << bf_.isNot(evaluateTo(c.arg(0)), result);
	return result;
}

unsigned int Compiler::and_fn(const Call& c, unsigned int result) {
	generated_ << bf_.logicalAnd(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), result);
	return result;
}

void Compiler::array_init(const Call& c) {
	unsigned int result = bf_.initArray(c.arg<Number>(1).getValue());
	generated_ << bf_.set(result, 0);
	vars_[c.arg<Variable>(0).getName()] = result;
}

void Compiler::array_set(const Call& c) {
	if (c.arg(1).getType() == Argument::INTEGER){
		auto target = bf_.getArrayPointer(getVar(c.arg<Variable>(0)), c.arg<Number>(1).getValue());
		if (c.arg(2).getType() == Argument::INTEGER)
			generated_ << bf_.set(target, c.arg<Number>(2).getValue());
		else
			generated_ << bf_.copy(evaluateTo(c.arg(2)), target);
	}
	else
		generated_ << bf_.arraySet(getVar(c.arg<Variable>(0)), evaluateTo(c.arg(1)), evaluateTo(c.arg(2)));
}

unsigned int Compiler::array_get(const Call& c, unsigned int result) {
	if (c.arg(1).getType() == Argument::INTEGER)
		generated_ << bf_.copy(bf_.getArrayPointer(getVar(c.arg<Variable>(0)), c.arg<Number>(1).getValue()), result);
	else
		generated_ << bf_.arrayGet(getVar(c.arg<Variable>(0)), evaluateTo(c.arg(1)), result);
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
	predef_methods[CallSignature(callname, args)] = std::bind(fptr, this, std::placeholders::_1);
}

void Compiler::reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, unsigned int (Compiler::*fptr) (const Call&, unsigned int)){
	predef_functions[CallSignature(callname, args)] = std::bind(fptr, this, std::placeholders::_1, std::placeholders::_2);
}

Compiler::Compiler(std::string code) {
	reg()
		("set", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::set)
		("add", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::add_const)
		("add", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::add_ev)
		("sub", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::sub_const)
		("sub", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::sub_ev)
		("div", { Argument::EVALUATABLE, Argument::EVALUATABLE, Argument::VARIABLE }, &Compiler::div)
		("mod", { Argument::EVALUATABLE, Argument::EVALUATABLE, Argument::VARIABLE }, &Compiler::mod)
		("print", { Argument::STRING }, &Compiler::print)
		("print", { Argument::EVALUATABLE }, &Compiler::print)
		//("printNumber", { Argument::EVALUATABLE }, &Compiler::printNumber)
		("getchar", { Argument::VARIABLE }, &Compiler::input)
		("if", { Argument::EVALUATABLE, Argument::CALLABLE, Argument::CALLABLE }, &Compiler::if_else_fn)
		("if", { Argument::EVALUATABLE, Argument::CALLABLE }, &Compiler::if_fn)
		("iseq", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::iseq)
		("isneq", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::isnoteq)
		("while", { Argument::EVALUATABLE, Argument::CALLABLE }, &Compiler::while_fn)
		("dowhile", { Argument::CALLABLE, Argument::EVALUATABLE }, &Compiler::do_while_fn)
		("not", { Argument::EVALUATABLE }, &Compiler::not_fn)
		("and", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::and_fn)
		("array_init", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::array_init)
		("array_get", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::array_get)
		("array_set", { Argument::VARIABLE, Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::array_set);

	//Remove comments from code before we do anything else
	code_ = remove_comments(code);
}

CompilerEasyRegister::CompilerEasyRegister(Compiler& owner) : owner_(owner) { }

CompilerEasyRegister& CompilerEasyRegister::operator () (std::string callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (const Call&)) {
	owner_.reg(callname, args, fptr);
	return *this;
}

CompilerEasyRegister& CompilerEasyRegister::operator () (std::string callname, const std::initializer_list<Argument::Type>& args, unsigned int (Compiler::*fptr) (const Call&, unsigned int)) {
	owner_.reg(callname, args, fptr);
	return *this;
}

std::string remove_comments(const std::string& code) {
	std::stringstream uncommentedCode;
	bool isString = false;
	bool isOneLineComment = false;
	bool isMultiLineComment = false;
	for (std::string::const_iterator i = std::begin(code); i != std::end(code); i++) {
		if (!isString) {
			if (!isOneLineComment && !isMultiLineComment) {
				if (i != std::end(code) - 1 && *i == '/' && *(i + 1) == '/')
					isOneLineComment = true;
				else if (i != std::end(code) - 1 && *i == '/' && *(i + 1) == '*')
					isMultiLineComment = true;
				else
					uncommentedCode << *i;
			}
			else if (*i == '\n')
				isOneLineComment = false;
			else if (i != std::end(code) - 1 && *i == '*' && *(i + 1) == '/'){
				isMultiLineComment = false;
				i++;
			}
		}
		else {
			uncommentedCode << *i;
		}
		if (*i == '"' && (i == std::begin(code) || *(i - 1) != '\\'))
			isString = !isString;
	}
	return uncommentedCode.str();
}
