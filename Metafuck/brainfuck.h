#include <string>
#include <vector>
#include "cell.h"
#ifndef BRAINFUCK
class Brainfuck
{
private:
   std::vector<Cell> cells_;
   unsigned int pointer_;
   
   void freeCell(unsigned int index);
   
public:
   unsigned int allocCell(unsigned int count);
   
   //Grundfunktionen
   std::string move(unsigned int to);
   std::string inc(unsigned int amount);
   std::string dec(unsigned int amount);
   std::string set(unsigned int index, unsigned int value);
   std::string copy(unsigned int source, unsigned int target);
   
   //IO
   std::string print(unsigned int index);
   std::string input(unsigned int target);
   
   //Addition
   std::string add(unsigned int index, unsigned int constant); //index + constant =: constant
   std::string addAway(unsigned int source, unsigned int target); //source + target =: target, source := 0
   std::string addCellTo(unsigned int a, unsigned int b, unsigned int target); //a + b =: target
   
   //Subtraktion
   
   //Komplexere Funktionen
   std::string printString(std::string s);

   Brainfuck();
};
#define BRAINFUCK
#endif