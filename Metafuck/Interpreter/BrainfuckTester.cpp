#include "BrainfuckTester.h"

namespace BrainfuckCompiler {
	BrainfuckTester::BrainfuckTester(std::string code, std::string input = "")
		:input_(input), output_(), program_(input_, output_)
	{
		
	}
}