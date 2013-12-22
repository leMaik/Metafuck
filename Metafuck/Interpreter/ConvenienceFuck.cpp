#include "ConvenienceFuck.h"

#include <iostream>
#include <fstream>

// test

void BinaryInput::get(char& c)
{
	if (front_) {
		c = data_.front();
		data_.pop_front();
	} else {
		c = data_.back();
		data_.pop_back();
	}
}

BinaryInput& BinaryInput::operator << (char c) 
{
	data_.push_back(c);

	return *this;
}
BinaryInput& BinaryInput::operator << (const char* str)
{
	for (char const* c = str; *c != '\0'; ++c) {
		data_.push_back(*c);
	}
	return *this;
}
BinaryInput& BinaryInput::operator << (std::string str)
{
	for (auto const& c : str) {
		data_.push_back(c);
	}
	return *this;
}
BinaryInput& BinaryInput::operator << (std::vector <char> data)
{
	for (auto const& c : data) {
		data_.push_back(c);
	}
	return *this;
}
BinaryInput& BinaryInput::operator << (std::deque <char> data)
{
	for (auto const& c : data) {
		data_.push_back(c);
	}
	return *this;
}

BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::size_t memorySize)
{
	return RunBrainfuckProgram<decltype(std::cin), decltype(std::cout)> (program, std::cin, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::stringstream& input, std::size_t memorySize)
{
	return RunBrainfuckProgram<std::stringstream, decltype(std::cout)> (program, input, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, BinaryInput& input, std::size_t memorySize)
{
	return RunBrainfuckProgram<BinaryInput, decltype(std::cout)> (program, input, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::string& input, std::size_t memorySize)
{
	std::stringstream sstr;
	sstr << input;
	return RunBrainfuckProgram<std::stringstream, decltype(std::cout)> (program, sstr, std::cout, memorySize);
}


BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullOutput(std::string program, std::size_t memorySize)
{
	NullOutput NullOut;
	return RunBrainfuckProgram<decltype(std::cin), NullOutput> (program, std::cin, NullOut, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullInput(std::string program, std::size_t memorySize)
{
	NullInput NullIn;
	return RunBrainfuckProgram<NullInput, decltype(std::cout)> (program, NullIn, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullIO(std::string program, std::size_t memorySize)
{
	NullOutput NullOut;
	NullInput NullIn;
	return RunBrainfuckProgram<NullInput, NullOutput> (program, NullIn, NullOut, memorySize);
}

BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::size_t memorySize)
{
	return RunBrainfuckProgram_FromFile<decltype(std::cin), decltype(std::cout)>(path, std::cin, std::cout, memorySize);
}

BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::string& input, std::size_t memorySize)
{
	std::stringstream sstr;
	sstr << input;
	return RunBrainfuckProgram_FromFile<std::stringstream, decltype(std::cout)>(path, sstr, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::stringstream& input, std::size_t memorySize)
{
	return RunBrainfuckProgram_FromFile<std::stringstream, decltype(std::cout)>(path, input, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, BinaryInput& input, std::size_t memorySize)
{
	return RunBrainfuckProgram_FromFile<BinaryInput, decltype(std::cout)> (path, input, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullInput(std::string path, std::size_t memorySize)
{
	NullInput NullIn;
	return RunBrainfuckProgram_FromFile<NullInput, decltype(std::cout)> (path, NullIn, std::cout, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullOutput(std::string path, std::size_t memorySize)
{
	NullOutput NullOut;
	return RunBrainfuckProgram_FromFile<decltype(std::cin), NullOutput> (path, std::cin, NullOut, memorySize);
}
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullIO(std::string path, std::size_t memorySize)
{
	NullOutput NullOut;
	NullInput NullIn;
	return RunBrainfuckProgram_FromFile<NullInput, NullOutput> (path, NullIn, NullOut, memorySize);
}