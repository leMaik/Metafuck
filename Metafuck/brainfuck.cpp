#include "brainfuck.h"
#include <sstream>

unsigned int Brainfuck::allocCell(unsigned int count) {
	//TODO: Das hier klappt nur für count == 1
	if (count == 1) {
		for (unsigned int i = 0; i < cells_.size(); i++) {
			if (!cells_[i].isInUse()) {
				cells_[i].setUsed(true);
				return i;
			}
		}
		//Okay, keine ungenutzte Zelle gefunden.
		cells_.push_back(Cell());
		return cells_.size() - 1;
	}
	else {
		//cout << "allocCell is not implemented for count > 1\ n";
		return -1;
	}
}

void Brainfuck::freeCell(unsigned int index) {
	if (cells_.size() > index) {
		cells_[index].setUsed(false);
	}
}

/*
Returns the brainfuck code to move the pointer to the given position. Warning: pointer_ is changed to the new position!
*/
std::string Brainfuck::move(unsigned int to) {
	std::stringstream result;
	if (to > pointer_) {
		do {
			result << ">";
			pointer_++;
		} while (pointer_ < to);
	}
	else if (to < pointer_) {
		do {
			result << "<";
			pointer_--;
		} while (pointer_ > to);
	}
	return result.str();
}

std::string Brainfuck::inc(unsigned int amount) {
	//TODO: Bei großem amount optimieren
	std::stringstream result;
	for (unsigned int i = 0; i < amount; i++)
		result << "+";
	return result.str();
}

std::string Brainfuck::dec(unsigned int amount) {
	//TODO: Bei großem amount optimieren
	std::string result = "";
	for (unsigned int i = 0; i < amount; i++)
		result += "-";
	return result;
}

std::string Brainfuck::set(unsigned int const &index, unsigned int const &value) {
	return move(index) + "[-]" + inc(value); //okay as inc() does not move the pointer_
}

std::string Brainfuck::copy(unsigned int source, unsigned int target) {
	unsigned int tmp = allocCell(1);
	std::stringstream r;
	r << set(tmp, 0);
	r << move(source) << "[";
	r << move(tmp) << "+";
	r << move(target) << "+";
	r << move(source) << "-]";
	r << move(tmp) << "[";
	r << move(source) << "+";
	r << move(tmp) << "-]";
	freeCell(tmp);
	return r.str();
}

std::string Brainfuck::print(unsigned int index) {
	return move(index) + ".";
}

std::string Brainfuck::printString(std::string s) {
	std::stringstream result;
	unsigned int tmp = allocCell(1);
	for (char c : s) {
		result << set(tmp, (unsigned int)c); //TODO: ASCII garantieren!
		result << print(tmp);
	}
	freeCell(tmp);
	return result.str();
}

std::string Brainfuck::input(unsigned int target) {
	return move(target) + ",";
}

std::string Brainfuck::add(unsigned int index, unsigned int constant) {
	std::stringstream result;
	result << move(index);
	result << inc(constant);
	return result.str();
}

std::string Brainfuck::addAway(unsigned int source, unsigned int target) {
	std::stringstream result;
	result << move(source);
	result << "[" << move(target) << "+";
	result << move(source) << "-]";
	return result.str();
}

std::string Brainfuck::addCellTo(unsigned int a, unsigned int b, unsigned int target) {
	std::stringstream result;
	unsigned int tmp = allocCell(1);
	result << copy(a, target);
	result << copy(b, tmp);
	result << addAway(tmp, target);
	freeCell(tmp);
	return result.str();
}

Brainfuck::Brainfuck() : pointer_(0) { }
