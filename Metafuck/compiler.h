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

typedef std::function<void(Compiler&, const Call&)> MfProcedure;
typedef std::function<unsigned int(Compiler&, const Call&, unsigned int target)> MfFunction;

class Compiler
{
private:
	Brainfuck bf_;
	std::string code_;
	CallList lexed_;
	std::map<std::string, unsigned int> vars_;

	std::map<CallSignature, MfProcedure> predef_methods;
	std::map<CallSignature, MfFunction> predef_functions;

	CompilerEasyRegister reg();

	bool isNumber(const std::string &s) const;
	bool isString(const std::string &s) const;

	void set(unsigned int target, Argument& evaluatable);

public:
	Compiler(std::string code, bool optimizeForSize);
	Compiler(const Compiler&) = delete;
	Compiler& operator=(const Compiler&) = delete;

	Brainfuck& bf() {
		return bf_;
	}

	std::stringstream generated_;

	bool validate();
	void lex();
	void compile();

	/*void add_const(const Call& c);
	void add_ev(const Call& c);
	void sub_const(const Call& c);
	void sub_ev(const Call& c);
	void div(const Call& c);
	void mod(const Call& c);
	void printNumber(const Call& c);

	unsigned int isnoteq(const Call& c, unsigned int result);
	unsigned int not_fn(const Call& c, unsigned int result);
	unsigned int and_fn(const Call& c, unsigned int result);
	unsigned int or_fn(const Call& c, unsigned int result);

	void array_init(const Call& c);
	void array_set(const Call& c);
	unsigned int array_get(const Call& c, unsigned int result);*/

	MfProcedure getProcedure(Call const& call);
	MfFunction getFunction(Call const& call);
	unsigned int getVar(const Variable& variable, bool ignoreDefining = false);

	void warning(Argument const* source, std::string message);

	inline void reg(const CallSignature& sig, const MfProcedure& proc) {
		predef_methods[sig] = proc;
	};

	inline void reg(const CallSignature& sig, const MfFunction& proc) {
		predef_functions[sig] = proc;
	};

	std::string getCode() const;
	std::string getGeneratedCode() const;
};

class CompilerEasyRegister {
public:
	CompilerEasyRegister(Compiler& owner);

	template<class... ArgTypes>
	CompilerEasyRegister& operator () (const std::string& callname, void(*fptr)(Compiler&, const ArgTypes&...)) {
		auto sig = CallSignature(callname, std::initializer_list<Type>{(ArgTypes::type)...});
		owner_.reg(sig, [&, fptr](Compiler& compiler, const Call& c){
			wrapper(compiler, c, fptr, indices_gen<sizeof...(ArgTypes)>{});
		});
		return *this;
	};

	template<class... ArgTypes>
	CompilerEasyRegister& operator () (const std::string& callname, unsigned int(*fptr)(Compiler&, unsigned int, const ArgTypes&...)) {
		auto sig = CallSignature(callname, std::initializer_list<Type>{(ArgTypes::type)...});
		owner_.reg(sig, [&, fptr](Compiler& compiler, const Call& c, unsigned int target)->unsigned int{
			return wrapper(compiler, c, fptr, indices_gen<sizeof...(ArgTypes)>{}, target);
		});
		return *this;
	};

private:
	Compiler& owner_;

	template<unsigned...> struct indices{};

	template<unsigned N, unsigned... Is>
	struct indices_gen : indices_gen<N - 1, N - 1, Is...>{};

	template<unsigned... Is>
	struct indices_gen<0, Is...> : indices<Is...>{};

	template<typename... Args, unsigned... Is>
	inline void wrapper(Compiler& compiler, const Call& c, void(*fptr)(Compiler&, const Args&...), indices<Is...>) {
		(*fptr)(compiler, static_cast<Args&>(c.arg(Is))...);
	}

	template<typename... Args, unsigned... Is>
	inline unsigned int wrapper(Compiler& compiler, const Call& c, unsigned int(*fptr)(Compiler&, unsigned int, const Args&...), indices<Is...>, unsigned int target) {
		return (*fptr)(compiler, target, static_cast<Args&>(c.arg(Is))...);
	}
};

std::string remove_comments(const std::string& code);
#endif
