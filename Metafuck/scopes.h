#ifndef SCOPES_H
#define SCOPES_H

#include <map>
#include <vector>
#include <string>

typedef unsigned int mfvar;

class Compiler;
class autoscope;

class scopes
{
	friend autoscope;
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

class autoscope
{
private:
	scopes& owner_;
public:
	autoscope(scopes& owner) : owner_(owner) {
		owner_.push();
	}
	~autoscope() {
		owner_.pop();
	}
};

#endif