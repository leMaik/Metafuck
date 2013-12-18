#include <string>
#ifndef CELL
class Cell {
private:
   std::string name_;
   int index_;
   bool inUse_;

public:
   Cell();

   // get set
   std::string getName() const;
   bool isInUse() const;
   void setUsed(bool inUse);

   // assignments
   //Cell& operator=(Cell const&) = delete;
   //Cell& operator=(Cell&&) = delete;

   // copy ctor
   //Cell(Cell&&) = delete;
   //Cell(Cell const&) = delete; //benötigt v. std::vector
};
#define CELL
#endif