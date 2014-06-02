#ifndef SCOPES_H
#define SCOPES_H

#include <map>
#include <vector>
#include <string>

typedef unsigned int mfvar;

class Compiler;

class scopes
{
private:
	Compiler& owner_;
	std::vector<std::map<std::string, mfvar>> scopes_;
public:
	scopes(Compiler& owner);
	~scopes() = default;
	void push();
	void pop();
	mfvar get(std::string name) const;
	mfvar set(std::string name, mfvar value);
	bool hasincscope(std::string name) const;
};

#endif