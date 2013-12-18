#include "cell.h"

Cell::Cell() : inUse_(true) {

}

std::string Cell::getName() const {
	return name_;
}

bool Cell::isInUse() const {
	return inUse_;
}

void Cell::setUsed(bool inUse) {
	inUse_ = inUse;
}