#ifndef COMPILER_H
#define COMPILER_H

#include "cell.h"
#include "brainfuck.h"
#include "Call.h"
#include "CallList.h"
#include "Variable.h"
#include "Functor.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <map>
#include <utility>

class CompilerEasyRegister;

class Compiler
{
private:
	Brainfuck bf_;
	std::string code_;
	std::stringstream generated_;
	CallList lexed_;
	std::map<CallSignature, TFunctor<Call&>*> predef_methods;
	std::map<std::string, unsigned int> vars_;

	unsigned int getVar(const Variable& variable);
	bool isNumber(const std::string &s) const;
	bool isString(const std::string &s) const;
	unsigned int evaluateTo(Argument& arg);
	void evaluate(Argument& arg);

public:
	Compiler(std::string c);

	CompilerEasyRegister& reg();
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (Call&));
	void reg(const std::string& callname, const std::initializer_list<Argument::Type>& args, int (Compiler::*fptr) (Call&));

	bool validate();
	std::size_t lex();
	void compile();

	void set(Call& c);
	void print(Call& c);

	std::string getCode() const;
	std::string getGeneratedCode() const;
};

void test(Compiler* me, Call c);

class CompilerEasyRegister {
public:
	CompilerEasyRegister(Compiler& owner);
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, void (Compiler::*fptr) (Call&));
	CompilerEasyRegister& operator () (std::string callname, const std::initializer_list<Argument::Type>& args, int (Compiler::*fptr) (Call&));
private:
	Compiler& owner_;
};
#endif