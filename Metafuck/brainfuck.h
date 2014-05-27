#ifndef BRAINFUCK
#define BRAINFUCK

#include <string>
#include <vector>

class Brainfuck
{
private:
	std::vector<bool> cells_;
	unsigned int pointer_;
	bool optimizeValueChanging_; //if this is true, multiple +/- will be replaced with shorter loops
	bool hasFreeCellsAt(unsigned int index, unsigned int count = 1) const;

public:
	unsigned int allocCell(unsigned int count = 1);
	unsigned int allocCellNear(unsigned int index, unsigned int count = 1);
	void freeCell(unsigned int index);

	//Grundfunktionen
	std::string move(unsigned int to);
	std::string inc(unsigned int amount);
	std::string dec(unsigned int amount);
	std::string set(unsigned int const index, unsigned int const value);
	std::string copy(unsigned int source, unsigned int target);

	//IO
	std::string print(unsigned int index);
	std::string input(unsigned int target);

	//Addition
	std::string add(unsigned int index, unsigned int constant); //index + constant =: index
	std::string addAway(unsigned int source, unsigned int target); //source + target =: target, source := 0
	std::string addCellTo(unsigned int a, unsigned int b, unsigned int target); //a + b =: target, target may be a or b.

	//Subtraktion
	std::string sub(unsigned int index, unsigned int constant); //index - constant =: index
	std::string subAway(unsigned int source, unsigned int target); //target - source =: target, source := 0
	std::string subCellFrom(unsigned int a, unsigned int b, unsigned int target); //a - b =: target, target may be a or b.

	//Division
	std::string divmod(unsigned int dividend, unsigned int devisor, unsigned int target);

	//Logik
	std::string isEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);
	std::string isNotEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);
	std::string isNot(unsigned int cell, unsigned int resultIndex);
	std::string logicalAnd(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);
	std::string logicalOr(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);

	//Komplexere Funktionen
	std::string printString(std::string s);
	std::string printNumber(unsigned int index);

	//Arrays
	unsigned int initArray(unsigned int length);
	std::string arraySet(unsigned int array, unsigned int index, unsigned int source);
	std::string arrayGet(unsigned int array, unsigned int index, unsigned int target);
	unsigned int getArrayPointer(unsigned int array, unsigned int index) const;

	Brainfuck(bool optimizeValueChanging = true);
	Brainfuck(const Brainfuck&) = delete;
	//Brainfuck& operator=(const Brainfuck&) = delete;
};
#endif
