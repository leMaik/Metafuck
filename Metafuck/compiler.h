#ifndef COMPILER_H
#define COMPILER_H

#include "brainfuck.h"
#include "Variable.h"
#include "Call.h"
#include "CallList.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <map>
#include <utility>
#include <functional>

class Compiler;
class CompilerEasyRegister;

typedef void(*MfProcedure) (const Call&, Compiler&, Brainfuck&);
typedef unsigned int(*MfFunction) (const Call&, Compiler&, Brainfuck&);

class Compiler
{
private:
	Brainfuck bf_;
	std::string code_;
	CallList lexed_;
	std::map<std::string, unsigned int> vars_;

	std::map<CallSignature, MfProcedure> predef_methods;
	std::map<CallSignature, MfFunction> predef_functions;

	bool isNumber(const std::string &s) const;
	bool isString(const std::string &s) const;
	unsigned int evaluateTo(Argument& arg);
	void evaluateTo(Argument& arg, unsigned int target);

	void set(unsigned int target, Argument& evaluatable);

public:
	Compiler(std::string code, bool optimizeForSize);

	std::stringstream generated_;

	bool validate();
	std::size_t lex();
	void compile();

	void set(const Call& c);
	void add_const(const Call& c);
	void add_ev(const Call& c);
	void sub_const(const Call& c);
	void sub_ev(const Call& c);
	void div(const Call& c);
	void mod(const Call& c);
	void print(const Call& c);
	void printNumber(const Call& c);
	void input(const Call& c);

	void if_fn(const Call& c);
	void if_else_fn(const Call& c);
	void while_fn(const Call& c);
	void do_while_fn(const Call& c);
	void for_fn(const Call& c);

	unsigned int iseq(const Call& c, unsigned int result);
	unsigned int isnoteq(const Call& c, unsigned int result);
	unsigned int not_fn(const Call& c, unsigned int result);
	unsigned int and_fn(const Call& c, unsigned int result);
	unsigned int or_fn(const Call& c, unsigned int result);

	void array_init(const Call& c);
	void array_set(const Call& c);
	unsigned int array_get(const Call& c, unsigned int result);

	MfProcedure getProcedure(Call const& call);
	MfFunction getFunction(Call const& call);
	unsigned int getVar(const Variable& variable);

	void warning(Argument const* source, std::string message);

	CompilerEasyRegister reg();
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, MfProcedure fptr);
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, MfFunction fptr);

	std::string getCode() const;
	std::string getGeneratedCode() const;
};

class CompilerEasyRegister {
public:
	CompilerEasyRegister(Compiler& owner);
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, MfProcedure fptr);
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, MfFunction fptr);
private:
	Compiler& owner_;
};

std::string remove_comments(const std::string& code);
#endif