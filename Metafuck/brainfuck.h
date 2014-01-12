#ifndef BRAINFUCK
#define BRAINFUCK

#include <string>
#include <vector>

class Brainfuck
{
private:
	std::vector<bool> cells_;
	unsigned int pointer_;

public:
	unsigned int allocCell(unsigned int count);
	unsigned int allocCellNear(unsigned int index);
	void freeCell(unsigned int index);

	//Grundfunktionen
	std::string move(unsigned int to);
	std::string inc(unsigned int amount);
	std::string dec(unsigned int amount);
	std::string set(unsigned int const &index, unsigned int const &value);
	std::string copy(unsigned int source, unsigned int target);

	//IO
	std::string print(unsigned int index);
	std::string input(unsigned int target);

	//Addition
	std::string add(unsigned int index, unsigned int constant); //index + constant =: index
	std::string addAway(unsigned int source, unsigned int target); //source + target =: target, source := 0
	std::string addCellTo(unsigned int a, unsigned int b, unsigned int target); //a + b =: target, target may be a or b.

	//Subtraktion

	//Logik
	std::string isEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);
	std::string isNotEqual(unsigned int indexA, unsigned int indexB, unsigned int resultIndex);
	std::string isNot(unsigned int cell, unsigned int resultIndex);

	//Komplexere Funktionen
	std::string printString(std::string s);

	Brainfuck();
};
#endif