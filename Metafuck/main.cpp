#include "compiler.h"
#include "brainfuck.h"
#include "Interpreter/ConvenienceFuck.h"
#include "NasmGenerator/BrainfuckNasmConverter.h"
#include "ookconv.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <stdlib.h>

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
			("output,o", po::value<std::string>(), "Output filename (default is <input filename>.bf)")
			("input,i", po::value<std::string>()->required(), "File to compile")
			("print,p", "Print the program on the screen after compiling (does not save it if --output is not set)")
			("run,r", "Run the program after compiling it")
			("nasm,n", po::value<std::string>(), "Convert generated brainfuck code to NASM (specify target platform by this parameter, may be win32 or unix)")
			("ook", "Output Ook! instead of brainfuck.");

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

		bool shallOptimize = !vm.count("nasm"); //TODO: add a command-line parameter for this

		Compiler com(mfstream.str(), shallOptimize);
		com.lex();
		com.compile();

		if (vm.count("nasm")) {
			TargetPlatform target = TargetPlatform::UNKNOWN;
			if (vm["nasm"].as<std::string>() == "win32")
				target = TargetPlatform::WIN32NT;
			else if (vm["nasm"].as<std::string>() == "unix")
				target = TargetPlatform::UNIX;

			if (target != TargetPlatform::UNKNOWN && (vm.count("output") || !vm.count("print"))) {
				std::cout << "___" << std::endl;

				std::ofstream out;
				std::string asmFilename = vm.count("output") ? vm["output"].as<std::string>() : vm["input"].as<std::string>() + ".asm";
				out.open(asmFilename);
				out << bf2nasm(com.getGeneratedCode(), target);
				out.close();

				if (target == TargetPlatform::WIN32NT){
					int nasm_res = system(("nasm -fobj \"" + asmFilename + "\" -o \"" + asmFilename + ".obj\"").c_str());
					if (nasm_res == 0) {
						int alink_res = system(("alink -oPE -subsys console \"" + asmFilename + ".obj\" -o \"" + asmFilename + ".exe\"").c_str());
						if (alink_res != 0) {
							std::cerr << "alink failed" << std::endl;
						}
						else if (vm.count("run")) {
							std::cout << "___" << std::endl << "Running:" << std::endl;
							system((asmFilename + ".exe").c_str());
						}
					}
					else {
						std::cerr << "nasm failed" << std::endl;
					}
				}
				else {
					int nasm_res = system(("nasm -felf \"" + asmFilename + "\" -o \"" + asmFilename + ".obj\"").c_str());
					if (nasm_res == 0) {
						int ld_res = system(("ld -melf_i386 -s -o \"" + asmFilename + ".out\" \"" + asmFilename + ".obj\"").c_str());
						if (ld_res != 0) {
							std::cerr << "ld failed" << std::endl;
						}
						else if (vm.count("run")) {
							std::cout << "___" << std::endl << "Running:" << std::endl;
							system((asmFilename + ".out").c_str());
						}
					}
					else {
						std::cerr << "nasm failed" << std::endl;
					}
				}
			}
		}
		else {
			if (vm.count("output") || !vm.count("print")) {
				std::ofstream out;
				out.open(vm.count("output") ? vm["output"].as<std::string>() : vm["input"].as<std::string>() + ".bf");
				if (vm.count("ook"))
					out << bf2ook(com.getGeneratedCode());
				else
					out << com.getGeneratedCode();
				out.close();
			}
		}

		if (vm.count("print")) {
			if (vm.count("ook"))
				std::cout << bf2ook(com.getGeneratedCode());
			else
				std::cout << com.getGeneratedCode();
		}

		if (vm.count("run") && !vm.count("nasm")) {
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
