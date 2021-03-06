#include "brainfuck.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

unsigned int Brainfuck::allocCell(unsigned int count) {
	return allocCellNear(pointer_, count);
}

Brainfuck::bftemp Brainfuck::allocCell_t(unsigned int count) {
    return Brainfuck::bftemp(*this, allocCell(count), count);
}

Brainfuck::bftemp Brainfuck::maketemp(unsigned int index, unsigned int count) {
    //std::cout << index << " is now temp" << std::endl;
    return Brainfuck::bftemp(*this, index, count);
}

unsigned int absdiff(unsigned int a, unsigned int b) {
	return a > b ? a - b : b - a;
}

bool Brainfuck::hasFreeCellsAt(unsigned int index, unsigned int count) const {
	for (unsigned int i = index; i <= std::min((decltype(cells_)::size_type)(index + count - 1), cells_.size() - 1); i++){
		if (cells_[i])
			return false;
	}
	return true;
}

unsigned int Brainfuck::allocCellNear(unsigned int index, unsigned int count) {
	unsigned int currBest = 0;
	bool found = false;
	for (unsigned int i = 0; i < cells_.size(); i++) {
		if (hasFreeCellsAt(i, count)) {
			if (!found || absdiff(i, index) < absdiff(index, currBest)){
				found = true;
				currBest = i;
			}
			else {
				break;
			}
		}
	}
	if (found) {
		for (unsigned int i = currBest; i < currBest + count; i++){
			if (cells_.size() > i)
				cells_[i] = true;
			else
				cells_.push_back(true);
		}
		return currBest;
	}
	else {
		//allocate cells at the end of the memory
		unsigned int index = cells_.size();
		while (count > 0){
			cells_.push_back(true);
			count--;
		}
		return index;
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
	if (amount == 0)
		return "";

	std::stringstream result;
	if (optimizeValueChanging_){
		auto tempCell = allocCell_t();
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
	}
	else {
		for (unsigned int i = 0; i < amount; i++)
			result << "+";
	}
	return result.str();
}

std::string Brainfuck::dec(unsigned int amount) {
	if (amount == 0)
		return "";

	std::stringstream result;
	if (optimizeValueChanging_){
		auto tempCell = allocCell_t();
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
	}
	else {
		for (unsigned int i = 0; i < amount; i++)
			result << "-";
	}
	return result.str();
}

std::string Brainfuck::set(unsigned int const index, unsigned int const value) {
	std::stringstream result;
	result << move(index) << "[-]";
	result << inc(value);
	return result.str();
}

std::string Brainfuck::copy(unsigned int source, unsigned int target) {
	auto tmp = allocCell_t();
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
	return r.str();
}

std::string Brainfuck::print(unsigned int index) {
	return move(index) + ".";
}

std::string Brainfuck::printString(std::string s) {
	std::stringstream result;
	auto tmp = allocCell_t();
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
	return result.str();
}

std::string Brainfuck::divmod(unsigned int dividend, unsigned int devisor, unsigned int target) {
	//target has to be the first of two consecutive cells
	std::stringstream result;
	auto tempFive = allocCell_t(10);
	result << copy(dividend, tempFive);
	result << set(tempFive + 1, 0);
	result << copy(devisor, tempFive + 2);
	result << move(tempFive) << "[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]";
	result << copy(tempFive + 3, target + 1);
	result << copy(tempFive + 4, target);
	return result.str();
}

std::string Brainfuck::printNumber(unsigned int index) {
	//std::stringstream result;
	//unsigned int temp = allocCell(2);

	//TODO

	//return result.str();
	return "";
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
	auto tmp = allocCell_t();
	if (a != target) {
		result << copy(a, target);
	}
	result << copy(b, tmp);
	result << addAway(tmp, target);
	return result.str();
}

std::string Brainfuck::sub(unsigned int index, unsigned int constant) {
	std::stringstream result;
	result << move(index);
	result << dec(constant);
	return result.str();
}

std::string Brainfuck::subAway(unsigned int source, unsigned int target) {
	std::stringstream result;
	result << move(source);
	result << "[" << move(target) << "-";
	result << move(source) << "-]";
	return result.str();
}

std::string Brainfuck::subCellFrom(unsigned int a, unsigned int b, unsigned int target) {
	std::stringstream result;
	auto tmp = allocCell_t();
	if (a != target) {
		result << copy(a, target);
	}
	result << copy(b, tmp);
	result << subAway(tmp, target);
	return result.str();
}

std::string Brainfuck::isEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex){
	std::stringstream result;
	auto tempIndexB = allocCell_t();
	result << copy(indexA, resultIndex);
	result << copy(indexB, tempIndexB);
	auto temp0 = allocCell_t();
	auto temp1 = allocCell_t();
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
	return result.str();
}

std::string Brainfuck::isNotEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex) {
	std::stringstream result;
	auto tempIndexB = allocCell_t();
	result << copy(indexA, resultIndex);
	result << copy(indexB, tempIndexB);
	auto temp0 = allocCell_t();
	auto temp1 = allocCell_t();
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
	return result.str();
}

std::string Brainfuck::isNot(unsigned int cell, unsigned int resultIndex) {
	std::stringstream result;
	auto temp = allocCell_t();
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

std::string Brainfuck::logicalAnd(unsigned int indexA, unsigned int indexB, unsigned int resultIndex) {
	std::stringstream result;
	auto tempB = allocCell_t();
	result << copy(indexB, tempB);
	auto temp0 = allocCell_t();
	auto temp1 = allocCell();
	result << set(temp0, 0);
	result << set(resultIndex, 0);
	result << copy(indexA, temp1);
	result << move(temp1) << "[";
	result << set(temp1, 0);
	result << move(tempB) << "[";
	result << move(temp1) << "+";
	result << move(temp0) << "+";
	result << move(tempB) << "-]";
	result << move(temp0) << "[";
	result << move(tempB) << "+";
	result << move(temp0) << "-]";
	result << move(temp1) << "[";
	result << move(resultIndex) << "+";
	result << set(temp1, 0) << "]]";

	return result.str();
}

std::string Brainfuck::logicalOr(unsigned int indexA, unsigned int indexB, unsigned int resultIndex) {
	std::stringstream result;
	auto temp0 = allocCell_t();
	auto temp1 = allocCell_t();

	result << copy(indexA, temp1);
	result << set(temp0, 0);
	result << set(resultIndex, 0);
	result << move(temp1) << "[";
	result << move(resultIndex) << "-";
	result << set(temp1, 0) << "]";
	result << copy(indexB, temp1);
	result << move(temp1) << "[";
	result << set(resultIndex, 0) << "-";
	result << set(temp1, 0) << "]";

	return result.str();
}

unsigned int Brainfuck::initArray(unsigned int length) {
	return allocCell(2 * length + 3);
}

std::string Brainfuck::arraySet(unsigned int array, unsigned int index, unsigned int source) {
	std::stringstream result;

	result << copy(index, array + 2);
	result << copy(source, array + 1);

	result << move(array);
	result << ">>[[>>]+[<<]>>-]+[>>]<[-]<[<<]>[>[>>]<+<[<<]>-]>[>>]<<[-<<]";
	//pointer is now at array, so everything is fine.

	return result.str();
}

std::string Brainfuck::arrayGet(unsigned int array, unsigned int index, unsigned int target) {
	std::stringstream result;

	result << set(target, 0);
	result << copy(index, array + 2);
	result << set(array + 1, 0);

	result << move(array);
	result << "y>>[[>>]+[<<]>>-]+[>>]<[<[<<]>+<";
	//pointer is now at array, so everything is fine.
	result << move(target) << "+";
	result << move(array);
	result << "y>>[>>]<-]<[<<]>[>[>>]<+<[<<]>-]>[>>]<<[-<<]";
	//pointer is now at array, so everything is still fine.

	return result.str();
}

unsigned int Brainfuck::getArrayPointer(unsigned int array, unsigned int index) const
{
	return 2 * index + 3;
}

Brainfuck::Brainfuck(bool optimizeValueChanging)
: pointer_(0), optimizeValueChanging_(optimizeValueChanging) {
}

Brainfuck::bftemp::bftemp(Brainfuck& bf, unsigned int index, unsigned int length)
    : bf_(bf), index_(index), length_(length) {
}

Brainfuck::bftemp::~bftemp() {
    //std::cout << "Deleted " << length_ << " temp cells at " << index_ << std::endl;
    for (unsigned int i=0;i < length_;++i)
        bf_.freeCell(i+index_);
}
