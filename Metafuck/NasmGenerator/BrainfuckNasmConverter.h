#ifndef BRAINFUCK_NASM_CONVERTER_H
#define BRAINFUCK_NASM_CONVERTER_H

#include <string>
#include <vector>
#include <memory>

enum TargetPlatform{
	UNIX,
	WIN32NT,
	UNKNOWN
};

std::string bf2nasm(std::string bf, TargetPlatform target);

class BrainfuckInstruction {
public:
	enum Type { CHANGE_VALUE, MOVE_POINTER, NONE };

	virtual std::string toNasm() = 0;
	virtual ~BrainfuckInstruction() = default;
};

typedef std::vector<std::shared_ptr<BrainfuckInstruction>> BrainfuckInstructions;

class ChangeValue : public BrainfuckInstruction {
public:
	void inc();
	void dec();
	std::string toNasm();
private:
	int count = 0;
};

class MovePointer : public BrainfuckInstruction {
public:
	void inc();
	void dec();
	std::string toNasm();
private:
	int count = 0;
};

class GetChar : public BrainfuckInstruction {
public:
	std::string toNasm();
};

class PrintChar : public BrainfuckInstruction {
public:
	std::string toNasm();
};

class ClearValue : public BrainfuckInstruction {
public:
	std::string toNasm();
};

class Loop : public BrainfuckInstruction {
public:
	std::string toNasm();
	Loop(unsigned int uniqueNumber, BrainfuckInstructions inner);
private:
	BrainfuckInstructions innerInstructions;
};
#endif