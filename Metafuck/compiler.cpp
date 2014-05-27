#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Number.h"
#include "helper.h"
#include "mfimpl/mfimpl.h"
#include <algorithm>
#include <locale>

std::size_t Compiler::lex() {
	lexed_ = CallList(code_);
	return 0;
}

unsigned int Compiler::getVar(const Variable& variable) {
	auto var = vars_.find(variable.getName());
	if (var == vars_.end()) {
		warning(&variable, "Implicitly defined variable '" + variable.getName() + "'");
		return (vars_[variable.getName()] = bf_.allocCell());
	}
	return var->second;
}

//Brainfuck& Compiler::bf() {
//	return bf_;
//}

unsigned int Compiler::evaluateTo(Argument& arg) {
	/*switch (arg.getType()){
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
	}*/
	return 0;
}

void Compiler::evaluateTo(Argument& arg, unsigned int target) {
	//switch (arg.getType()){
	//case Argument::CALL:
	//{
	//					   Call& c = static_cast<Call&>(arg);
	//					   auto function = std::find_if(std::begin(predef_functions), std::end(predef_functions),
	//						   [&c](std::pair<CallSignature, std::function<int(const Call&, unsigned int)>> k) -> bool {
	//						   return c.matches(k.first);
	//					   });
	//					   if (function != predef_functions.end()){
	//						   function->second(c, target);
	//					   }
	//					   else {
	//						   std::cerr << "Unknown function: " << c << std::endl;
	//					   }
	//}
	//	break;
	//case Argument::Type::VARIABLE:
	//	//TODO: In this case, the produced variable is temporary and should be treated as such
	//	generated_ << bf_.copy(getVar(static_cast<Variable&>(arg)), target);
	//	break;
	//case Argument::Type::INTEGER:
	//	generated_ << bf_.set(target, static_cast<Number&>(arg).getValue());
	//	break;
	//default:
	//	//TODO: Exception
	//	break;
	//}
}

void Compiler::set(const Call& c) {
	set(getVar(c.arg<Variable>(0)), c.arg(1));
}

void Compiler::set(unsigned int target, Argument& evaluatable){
	//generates the code that sets ´target´ to the evaluated value of ´evaluated´

	if (evaluatable.getType() == Argument::INTEGER)
		generated_ << bf_.set(target, static_cast<Number&>(evaluatable).getValue());
	else
		evaluateTo(evaluatable, target);
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

unsigned int Compiler::or_fn(const Call& c, unsigned int result) {
	generated_ << bf_.logicalOr(evaluateTo(c.arg(0)), evaluateTo(c.arg(1)), result);
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

MfProcedure Compiler::getProcedure(Call const& call)
{
	auto method = std::find_if(std::begin(predef_methods), std::end(predef_methods),
		[&call](std::pair<CallSignature, MfProcedure> k) -> bool {
		return call.matches(k.first);
	});
	if (method != std::end(predef_methods)){
		return method->second;
	}
	return nullptr;
}

MfFunction Compiler::getFunction(Call const& call)
{
	auto method = std::find_if(std::begin(predef_functions), std::end(predef_functions),
		[&call](std::pair<CallSignature, MfFunction> k) -> bool {
		return call.matches(k.first);
	});
	if (method != std::end(predef_functions)){
		return method->second;
	}
	return nullptr;
}

void Compiler::warning(Argument const* source, std::string message) {
	std::cout << "[WARNING] " << message << std::endl;
}

void Compiler::compile() {
	lexed_.compile(*this);
}

std::string Compiler::getCode() const {
	return code_;
}

std::string Compiler::getGeneratedCode() const {
	return generated_.str();
}

Compiler::Compiler(std::string code, bool optimizeForSize) : bf_(optimizeForSize) {
	reg()
		//("set", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::set)
		//("add", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::add_const)
		//("add", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::add_ev)
		//("sub", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::sub_const)
		//("sub", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::sub_ev)
		//("div", { Argument::EVALUATABLE, Argument::EVALUATABLE, Argument::VARIABLE }, &Compiler::div)
		//("mod", { Argument::EVALUATABLE, Argument::EVALUATABLE, Argument::VARIABLE }, &Compiler::mod)
		("print", &metafuck::impl::io::print_str)
		("print", &metafuck::impl::io::print_var)
		("getchar", &metafuck::impl::io::getchar);
	//("print", { Argument::EVALUATABLE }, &metafuck::impl::io::print)
	////("printNumber", { Argument::EVALUATABLE }, &Compiler::printNumber)
	//	("getchar", { Argument::VARIABLE }, &metafuck::impl::io::getchar)
	//("if", { Argument::EVALUATABLE, Argument::CALLABLE, Argument::CALLABLE }, &Compiler::if_else_fn)
	//("if", { Argument::EVALUATABLE, Argument::CALLABLE }, &Compiler::if_fn)
	//("iseq", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::iseq)
	//("isneq", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::isnoteq)
	//("while", { Argument::EVALUATABLE, Argument::CALLABLE }, &Compiler::while_fn)
	//("dowhile", { Argument::CALLABLE, Argument::EVALUATABLE }, &Compiler::do_while_fn)
	//("for", { Argument::VARIABLE, Argument::EVALUATABLE, Argument::EVALUATABLE, Argument::CALLABLE, Argument::CALLABLE }, &Compiler::for_fn)
	//("not", { Argument::EVALUATABLE }, &Compiler::not_fn)
	//("and", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::and_fn)
	//("or", { Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::or_fn)
	//("array_init", { Argument::VARIABLE, Argument::INTEGER }, &Compiler::array_init)
	//("array_get", { Argument::VARIABLE, Argument::EVALUATABLE }, &Compiler::array_get)
	//("array_set", { Argument::VARIABLE, Argument::EVALUATABLE, Argument::EVALUATABLE }, &Compiler::array_set);
	//;
	//Remove comments from code before we do anything else
	code_ = remove_comments(code);
}

CompilerEasyRegister Compiler::reg() {
	return CompilerEasyRegister(*this);
}
//
//void Compiler::reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, MfFunction fptr){
//	predef_functions[CallSignature(callname, args)] = fptr;
//}
//
CompilerEasyRegister::CompilerEasyRegister(Compiler& owner) : owner_(owner) {
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
