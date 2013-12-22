#ifndef CONVENIENCE_FUCK_H_INCLUDED
#define CONVENIENCE_FUCK_H_INCLUDED

#include "PrecomFuck.h"

#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <fstream>

class NullInput
{
public:
	NullInput() = default;
	NullInput(NullInput&) = default;
	NullInput& operator=(NullInput&) = default;

	void get(char& c) { c = '\0'; }
};

class NullOutput
{
public:
	NullOutput() = default;
	NullOutput(NullOutput&) = default;
	NullOutput& operator=(NullOutput&) = default;

	void put(char& c) { /* no operation */ }
};

class BinaryInput
{
public:
	BinaryInput(bool from_front = true) : data_(), front_(from_front) {}
	BinaryInput(std::deque<char> data, bool from_front) : data_(std::move(data)), front_(from_front) {}
	BinaryInput(std::vector<char> const& data, bool from_front) : data_(data.begin(), data.end()), front_(from_front) {}
	BinaryInput(std::string const& data, bool from_front) : data_(data.begin(), data.end()), front_(from_front) {}

	BinaryInput& operator << (char c);
	BinaryInput& operator << (const char* str);
	BinaryInput& operator << (std::string str);
	BinaryInput& operator << (std::vector <char> data);
	BinaryInput& operator << (std::deque <char> data);

	void get(char& c);
private:
	std::deque <char> data_;
	bool front_;
};

template <typename InputStreamT, typename OutputStreamT>
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, InputStreamT& is, OutputStreamT& os, std::size_t memorySize = 1<<16)
{
	using namespace BrainfuckCompiler;

	BrainfuckProgram<InputStreamT, OutputStreamT> prog(is, os);
	VirtualMemory vmem(memorySize);
	BrainfuckPrecompiler Precom;
	auto error = Precom.verify(program);
	if (error == CEC_NO_ERROR)
	{
		Precom.compile(program, prog);
		try {
			prog(vmem);
			return CEC_NO_ERROR;
		} catch (...) {
			return CEC_RUNTIME_ERROR;
		}
	}
	else
		return error;
}

// on cin cout
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::size_t memorySize = 1 << 16);

// custom input on cout
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::stringstream& input, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, std::string& input, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram(std::string program, BinaryInput& input, std::size_t memorySize = 1 << 16);

// on cin / cout with one or both channels nulled / ignored
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullOutput(std::string program, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullInput(std::string program, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_NullIO(std::string program, std::size_t memorySize = 1 << 16);

// same as all functions above, but load program from file
template <typename InputStreamT, typename OutputStreamT>
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, InputStreamT& is, OutputStreamT& os, std::size_t memorySize = 1<<16)
{
	std::ifstream reader (path);
	std::stringstream buffer;
	buffer << reader.rdbuf();

	return RunBrainfuckProgram<InputStreamT, OutputStreamT>(buffer.str(), is, os, memorySize);
}

BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::string& input, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, std::stringstream& input, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile(std::string path, BinaryInput& input, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullInput(std::string path, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullOutput(std::string path, std::size_t memorySize = 1 << 16);
BrainfuckCompiler::CompilerErrorCode RunBrainfuckProgram_FromFile_NullIO(std::string path, std::size_t memorySize = 1 << 16);
#endif
