#ifndef COMPILER_H
#define COMPILER_H

#include "brainfuck.h"
#include "Call.h"
#include "CallList.h"
#include "Variable.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <map>
#include <utility>
#include <functional>

class CompilerEasyRegister;

class Compiler
{
private:
	Brainfuck bf_;
	std::string code_;
	std::stringstream generated_;
	CallList lexed_;
	std::map<CallSignature, std::function<void(const Call&)>> predef_methods;
	std::map<CallSignature, std::function<unsigned int(const Call&)>> predef_functions;
	std::map<std::string, unsigned int> vars_;

	unsigned int getVar(const Variable& variable);
	bool isNumber(const std::string &s) const;
	bool isString(const std::string &s) const;
	unsigned int evaluateTo(Argument& arg);
	void evaluateTo(Argument& arg, unsigned int target);
	void evaluate(Argument& arg);

public:
	Compiler(std::string code);

	CompilerEasyRegister reg();
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (const Call&));
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, unsigned int (Compiler::*fptr) (const Call&));

	bool validate();
	std::size_t lex();
	void compile();

	void set(const Call& c);
	void add_const(const Call& c);
	void div(const Call& c);
	void mod(const Call& c);
	void print(const Call& c);
	void printNumber(const Call& c);
	void input(const Call& c);
	void if_fn(const Call& c);
	void if_else_fn(const Call& c);
	void while_fn(const Call& c);
	void do_while_fn(const Call& c);

	unsigned int iseq(const Call& c);
	unsigned int isnoteq(const Call& c);
	unsigned int not_fn(const Call& c);
	unsigned int and_fn(const Call& c);

	void array_init(const Call& c);
	void array_set(const Call& c);
	unsigned int array_get(const Call& c);

	std::string getCode() const;
	std::string getGeneratedCode() const;
};

class CompilerEasyRegister {
public:
	CompilerEasyRegister(Compiler& owner);
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (const Call&));
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, unsigned int (Compiler::*fptr) (const Call&));
private:
	Compiler& owner_;
};

std::string remove_comments(const std::string& code);
#endif