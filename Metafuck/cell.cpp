#include "cell.h"
/*
Cell::Cell(std::string name = null) : name_(std::move(name)), index_(0) {

}*/

std::string Cell::getName() const {
	return name_;
}

bool Cell::isInUse() const {
	return inUse_;
}

void Cell::setUsed(bool inUse) {
	inUse_ = inUse;
}
