#ifndef BRAINFUCKTESTER_H_INCLUDED
#define BRAINFUCKTESTER_H_INCLUDED

#include "PrecomFuck.h"

#include <string>
#include <sstream>

namespace BrainfuckCompiler {

	class BrainfuckTester {
	public:
		BrainfuckTester(std::string code, std::string input = "");

	private:
		std::string input_;
		std::stringstream output_;
		BrainfuckProgram <std::string, std::stringstream> program_;
	};

} // namespace BrainfuckCompiler

#endif // BRAINFUCKTESTER_H_INCLUDED