#include "compiler.h"
#include "CallList.h"
#include "Call.h"
#include "String.h"
#include "Number.h"
#include "helper.h"
#include "mfimpl/mfimpl.h"
#include <algorithm>
#include <locale>

void Compiler::lex() {
	lexed_ = CallList(code_);
}

unsigned int Compiler::getVar(const Variable& variable, bool ignoredef) {
	auto var = vars_.find(variable.getName());
	if (var == vars_.end()) {
		if (!ignoredef)
			warning(&variable, "Implicitly defined variable '" + variable.getName() + "'");
		return (vars_[variable.getName()] = bf_.allocCell());
	}
	return var->second;
}

void Compiler::setVar(const Variable& variable, unsigned int value) {
	auto var = vars_.find(variable.getName());
	if (var != vars_.end())
		warning(&variable, "Redefining previously defined variable " + variable.getName() + " may result in unexpected behaviour");
	vars_[variable.getName()] = value;
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
	warnings++;
}

void Compiler::error(Argument const* source, std::string message) {
	std::cout << "[ERROR] " << message << std::endl;
	errors++;
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
		("set", &metafuck::impl::basic::set)
		("add", &metafuck::impl::math::add_const)
		("add", &metafuck::impl::math::add_ev)
		("sub", &metafuck::impl::math::sub_const)
		("sub", &metafuck::impl::math::sub_ev)
		("div", &metafuck::impl::math::div)
		("mod", &metafuck::impl::math::mod)
		("print", &metafuck::impl::io::print_str)
		("print", &metafuck::impl::io::print_var)
		("getchar", &metafuck::impl::io::getchar)
		////("printNumber", { Argument::EVALUATABLE }, &Compiler::printNumber)
		("if", &metafuck::impl::flow::if_else_fn)
		("if", &metafuck::impl::flow::if_fn)
		("iseq", &metafuck::impl::logic::iseq)
		("isneq", &metafuck::impl::logic::isnoteq)
		("while", &metafuck::impl::flow::while_fn)
		("dowhile", &metafuck::impl::flow::do_while_fn)
		("for", &metafuck::impl::flow::for_fn)
		("not", &metafuck::impl::logic::not)
		("and", &metafuck::impl::logic::and)
		("or", &metafuck::impl::logic::or)
		("array_init", &metafuck::impl::array::init)
		("array_get", &metafuck::impl::array::get)
		("array_set", &metafuck::impl::array::set)
		;

	code_ = remove_comments(code);
}

CompilerEasyRegister Compiler::reg() {
	return CompilerEasyRegister(*this);
}

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
