#include "compiler.h"
#include "brainfuck.h"
#include "Interpreter\ConvenienceFuck.h"
#include "boost/program_options.hpp" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>

namespace
{
	const size_t ERROR_IN_COMMAND_LINE = 1;
	const size_t SUCCESS = 0;
	const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}

int main(int argc, char** argv)
{
	try
	{
		// Define and parse the program options
		namespace po = boost::program_options;

		po::options_description desc("Options");
		desc.add_options()
			("help", "Print help messages")
			("output,o", po::value<std::string>(), "Output filename (default is <input filename>.bf")
			("input,i", po::value<std::string>()->required(), "File to compile")
			("print,p", "Print the program on the screen after compiling (does not save it if --output is not set)")
			("run,r", "Run the program after compiling it");

		po::positional_options_description positionalOptions;
		positionalOptions.add("input", 1);

		po::variables_map vm;
		try
		{
			po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), vm);

			// --help
			if (vm.count("help"))
			{
				std::cout << "Metafuck Compiler" << std::endl
					<< desc << std::endl;
				return SUCCESS;
			}

			po::notify(vm); // throws on error
		}
		catch (po::error& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return ERROR_IN_COMMAND_LINE;
		}

		// Application code
		std::ifstream in;
		in.open(vm["input"].as<std::string>());
		std::stringstream mfstream;
		mfstream << in.rdbuf();//read the file
		in.close();

		Compiler com(mfstream.str());
		com.lex();
		com.compile();
		//com.optimize();

		if (vm.count("output") || !vm.count("print")) {
			std::ofstream out;
			out.open(vm.count("output") ? vm["output"].as<std::string>() : vm["input"].as<std::string>() + ".bf");
			out << com.getGeneratedCode();
			out.close();
		}
		if (vm.count("print")) {
			std::cout << com.getGeneratedCode() << std::endl;
		}

		if (vm.count("run")) {
			std::cout << "___" << std::endl << "Running:" << std::endl;
			RunBrainfuckProgram(com.getGeneratedCode());
		}

		return SUCCESS;

	}
	catch (std::exception& e)
	{
		std::cerr << "Unhandled Exception reached the top of main: "
			<< e.what() << ", application will now exit" << std::endl;
		return ERROR_UNHANDLED_EXCEPTION;

	}
}