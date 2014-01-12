#include "brainfuck.h"
#include <sstream>
#include <cmath>

unsigned int Brainfuck::allocCell(unsigned int count) {
	return allocCellNear(pointer_);
	//TODO: Das hier klappt nur für count == 1
	if (count == 1) {
		for (unsigned int i = 0; i < cells_.size(); i++) {
			if (!cells_[i]) {
				cells_[i] = true;
				return i;
			}
		}
		//Okay, keine ungenutzte Zelle gefunden.
		cells_.push_back(true);
		return cells_.size() - 1;
	}
	else {
		//cout << "allocCell is not implemented for count > 1\ n";
		return -1;
	}
}

unsigned int absdiff(unsigned int &a, unsigned int &b) {
	return a > b ? a - b : b - a;
}

unsigned int Brainfuck::allocCellNear(unsigned int index) {
	unsigned int currBest;
	bool found = false;
	for (unsigned int i = 0; i < cells_.size(); i++) {
		if (!cells_[i]) {
			found = true;
			if (absdiff(i, index) < absdiff(index, currBest) || !found){
				currBest = i;
			}
			else {
				break;
			}
		}
	}
	if (found){
		cells_[currBest] = true;
		return currBest;
	}
	else {
		cells_.push_back(true);
		return cells_.size() - 1;
	}
}

void Brainfuck::freeCell(unsigned int index) {
	if (cells_.size() > index) {
		cells_[index] = false;
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
	std::stringstream result;
	unsigned int tempCell = allocCellNear(pointer_);
	unsigned int prt = (unsigned int)sqrt(amount);
	if (absdiff(pointer_, tempCell) * 2 + 8 + 2 * prt + (amount - prt*prt) < amount) {
		unsigned int p = pointer_;
		if (prt*prt < amount){
			result << inc(amount - prt*prt);
		}
		result << set(tempCell, prt);
		result << move(tempCell) << "[";
		result << move(p);
		for (unsigned int i = 0; i < prt; i++)
			result << "+";
		result << move(tempCell) << "-]";
	}
	else {
		for (unsigned int i = 0; i < amount; i++)
			result << "+";
	}
	freeCell(tempCell);
	return result.str();
}

std::string Brainfuck::dec(unsigned int amount) {
	std::stringstream result;
	unsigned int tempCell = allocCellNear(pointer_);
	unsigned int prt = (unsigned int)std::sqrt(amount);
	if (absdiff(pointer_, tempCell) * 2 + 8 + 2 * prt + (amount - prt*prt) < amount) {
		unsigned int p = pointer_;
		if (prt*prt < amount){
			result << dec(amount - prt*prt);
		}
		result << set(tempCell, prt);
		result << move(tempCell) << "[";
		result << move(p);
		for (unsigned int i = 0; i < prt; i++)
			result << "-";
		result << move(tempCell) << "-]";
	}
	else {
		for (unsigned int i = 0; i < amount; i++)
			result << "-";
	}
	freeCell(tempCell);
	return result.str();
}

std::string Brainfuck::set(unsigned int const &index, unsigned int const &value) {
	std::stringstream result;
	result << move(index) << "[-]";
	result << inc(value);
	return result.str();
}

std::string Brainfuck::copy(unsigned int source, unsigned int target) {
	unsigned int tmp = allocCell(1);
	std::stringstream r;
	r << set(tmp, 0);
	r << set(target, 0);
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
	result << set(tmp, 0);
	char prev = (char)0;
	for (char c : s) {
		if ((int)c > (int)prev){
			result << add(tmp, (int)c - (int)prev);
		}
		else {
			//TODO: use something like sub() later
			result << move(tmp);
			result << dec((int)prev - (int)c);
		}
		prev = c;
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
	if (a != target) {
		result << copy(a, target);
	}
	result << copy(b, tmp);
	result << addAway(tmp, target);
	freeCell(tmp);
	return result.str();
}

std::string Brainfuck::isEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex){
	std::stringstream result;
	unsigned int tempIndexB = allocCell(1);
	result << copy(indexA, resultIndex);
	result << copy(indexB, tempIndexB);
	unsigned int temp0 = allocCell(1);
	unsigned int temp1 = allocCell(1);
	result << set(temp0, 0);
	result << set(temp1, 0);
	result << addAway(resultIndex, temp1);
	result << add(resultIndex, 1);
	result << move(tempIndexB) << "[";
	result << move(temp1) << "-";
	result << move(temp0) << "+";
	result << move(tempIndexB) << "-]";
	result << move(temp0) << "[";
	result << move(tempIndexB) << "+";
	result << move(temp0) << "-]";
	result << move(temp1) << "[";
	result << move(resultIndex) << "-";
	result << set(temp1, 0) << "]";
	freeCell(temp0);
	freeCell(temp1);
	return result.str();
}

std::string Brainfuck::isNotEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex) {
	std::stringstream result;
	unsigned int tempIndexB = allocCell(1);
	result << copy(indexA, resultIndex);
	result << copy(indexB, tempIndexB);
	unsigned int temp0 = allocCell(1);
	unsigned int temp1 = allocCell(1);
	result << set(temp0, 0);
	result << set(temp1, 0);
	result << move(resultIndex) << "[";
	result << move(temp1) << "+";
	result << move(resultIndex) << "-]";
	result << move(tempIndexB) << "[";
	result << move(temp1) << "-";
	result << move(temp0) << "+";
	result << move(tempIndexB) << "-]";
	result << move(temp0) << "[";
	result << move(tempIndexB) << "+";
	result << move(temp0) << "-]";
	result << move(temp1) << "[";
	result << move(resultIndex) << "+";
	result << set(temp1, 0) << "]";
	freeCell(temp0);
	freeCell(temp1);
	return result.str();
}

std::string Brainfuck::isNot(unsigned int cell, unsigned int resultIndex) {
	std::stringstream result;
	unsigned int temp = allocCell(0);
	result << set(temp, 0);
	result << copy(cell, resultIndex);
	result << move(resultIndex) << "[";
	result << move(temp) << "+";
	result << set(resultIndex, 0) << "]+";
	result << move(temp) << "[";
	result << move(resultIndex) << "-";
	result << move(temp) << "-]";
	return result.str();
}

Brainfuck::Brainfuck() : pointer_(0) { }
