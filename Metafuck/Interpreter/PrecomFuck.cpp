#include "PrecomFuck.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <exception>

//#############################################################
std::string ValidInstructions = "+-.,<>[]";
//#############################################################
namespace BrainfuckCompiler {
//#############################################################
	VirtualMemory::VirtualMemory(std::size_t staticSize)
	:memory(staticSize, '\0'), size(staticSize)
	{

	}
	CELL_TYPE* VirtualMemory::getMemoryPointer()
	{
		return &*memory.begin();
	}
	std::size_t VirtualMemory::getSize() const
	{
		return size;
	}
//#############################################################
	IncrementInstruction::IncrementInstruction(unsigned int amount)
	:amount_(amount)
	{
	}
	void IncrementInstruction::execute(CELL_TYPE*& ptr) const
	{
		*ptr = *ptr + amount_;
	}
//#############################################################
	DecrementInstruction::DecrementInstruction(unsigned int amount)
	:amount_(amount)
	{
	}
	void DecrementInstruction::execute(CELL_TYPE*& ptr) const
	{
		*ptr = *ptr - amount_;
	}
//#############################################################
	MemoryPointerShift::MemoryPointerShift(int amount)
	:amount_(amount)
	{
	}
	void MemoryPointerShift::execute(CELL_TYPE*& ptr) const
	{
		ptr += amount_;
	}
//#############################################################
	ConditionalJumpForward::ConditionalJumpForward(std::function <void(std::size_t)> jumper, std::size_t instructionPtr)
	:jumper_(jumper), instructionPtr_(instructionPtr)
	{
	}
	//-------------------------------------------------------------
	void ConditionalJumpForward::execute(CELL_TYPE*& ptr) const
	{
		if (*ptr == 0)
			jumper_(instructionPtr_);
	}
	//-------------------------------------------------------------
	void ConditionalJumpForward::resetInstructionPtr(std::size_t instructionPtr)
	{
		instructionPtr_ = instructionPtr;
	}
//#############################################################
	ConditionalJumpBackward::ConditionalJumpBackward(std::function <void(std::size_t)> jumper, std::size_t instructionPtr)
	:jumper_(jumper), instructionPtr_(instructionPtr)
	{
	}
	//-------------------------------------------------------------
	void ConditionalJumpBackward::execute(CELL_TYPE*& ptr) const
	{
		if (*ptr != 0)
			jumper_(instructionPtr_);
	}
	//-------------------------------------------------------------
	void ConditionalJumpBackward::resetInstructionPtr(std::size_t instructionPtr)
	{
		instructionPtr_ = instructionPtr;
	}
//#############################################################
	CompilerErrorCode BrainfuckPrecompiler::verify(std::string const& str)
	{
		int counter = 0;
		int mem_counter = 0;
		for (auto const& i : str) {
			switch (i)
			{
				case('['): counter++; break;
				case(']'): counter--; break;
				case('<'): mem_counter--; break;
				case('>'): mem_counter++; break;
				default: break;
			}
			if (counter < 0)
				return CEC_LOOP_MISMATCH;
			//if (mem_counter < 0)
			//	return CEC_SEG_FAULT;
		}
		return CEC_NO_ERROR;
	}
	//-------------------------------------------------------------
	bool BrainfuckPrecompiler::isInstruction(CELL_TYPE c) const
	{
		return std::find(std::begin(ValidInstructions), std::end(ValidInstructions), c) != std::end(ValidInstructions);
	}
	//-------------------------------------------------------------
//#############################################################
} // Brainfuck Compiler
