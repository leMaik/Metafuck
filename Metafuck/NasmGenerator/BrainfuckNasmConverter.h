#ifndef BRAINFUCK_NASM_CONVERTER_H
#define BRAINFUCK_NASM_CONVERTER_H

#include <string>

enum TargetPlatform{
	UNIX,
	WIN32NT,
	UNKNOWN
};

std::string bf2nasm(std::string bf, TargetPlatform target);
#endif