#include "scopes.h"
#include "compiler.h"
#include <exception>

scopes::scopes(Compiler& owner)
	: owner_(owner) {
}

void scopes::push() {
	scopes_.push_back(std::map<std::string, mfvar>());
}

void scopes::pop() {
	//free all vars in the current scope
	for (auto& var : scopes_.back()) {
		owner_.bf().freeCell(var.second);
	}
	std::cout << "scope removed" << std::endl;
	scopes_.pop_back();
}

mfvar scopes::get(std::string name) const {
	for (auto& scope = scopes_.rbegin(); scope != scopes_.rend(); ++scope) {
		auto& var = scope->find(name);
		if (var != scope->end())
			return var->second;
	}

	throw std::runtime_error("variable " + name + " does not exist");
}

mfvar scopes::set(std::string name, mfvar value) {
	return scopes_.back()[name] = value;
}

bool scopes::hasincscope(std::string name) const {
	auto& var = scopes_.back().find(name);
	return var != scopes_.back().end();
}