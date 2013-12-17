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

std::string Brainfuck::move(unsigned int to) {
	std::string result = "";
	if (to > pointer_) {
		for (; pointer_ < to; pointer_++)
			result += ">";
	}
	else if (to < pointer_) {
		for (; pointer_ > to; pointer_--)
			result += "<";
	}
	return result;
}

std::string Brainfuck::inc(unsigned int amount) {
	//TODO: Bei großem amount optimieren
	std::string result = "";
	for (int i = 0; i < amount; i++)
		result += "+";
	return result;
}

std::string Brainfuck::dec(unsigned int amount) {
	//TODO: Bei großem amount optimieren
	std::string result = "";
	for (int i = 0; i < amount; i++)
		result += "-";
	return result;
}

std::string Brainfuck::set(unsigned int index, unsigned int value) {
	return move(index) + "[-]" + inc(value);
}

std::string Brainfuck::copy(unsigned int source, unsigned int target) {
	unsigned int tmp = allocCell(1);
	std::stringstream result;
	result << set(tmp, 0) << move(source) << "[" << move(tmp) << "+" << move(target) << "+" << move(source) << "-]" << move(tmp) << "[" << move(source) << "+" << move(tmp) << "-]";
	freeCell(tmp);
	return result.str();
}

std::string Brainfuck::print(unsigned int index) {
	return move(index) + ".";
}

std::string Brainfuck::printString(std::string s) {
	std::stringstream result;
	unsigned int tmp = allocCell(1);
	for (char c : s) {
		//TODO: ASCII garantieren!!!!
		result << set(tmp, (int)c) << print(tmp);
	}
	freeCell(tmp);
	return result.str();
}

std::string Brainfuck::input(unsigned int target) {
	return move(target) + ",";
}

std::string Brainfuck::add(unsigned int index, unsigned int constant) {
	return move(index) + inc(constant);
}

std::string Brainfuck::addAway(unsigned int source, unsigned int target) {
	return move(source) + "[" + move(target) + "+" + move(source) + "-]";
}

std::string Brainfuck::addCellTo(unsigned int a, unsigned int b, unsigned int target) {
	unsigned int tmp = allocCell(1);
	std::string result = copy(a, target) + copy(b, tmp) + addAway(tmp, target);
	freeCell(tmp);
	return result;
}

Brainfuck::Brainfuck() : pointer_(0) { }