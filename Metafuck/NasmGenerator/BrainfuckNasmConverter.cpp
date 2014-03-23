#include "BrainfuckNasmConverter.h"
#include <sstream>
#include <stack>

std::string bf2nasm(std::string bf, TargetPlatform target)
{
	std::stringstream nasm;
	unsigned int nextLoop = 0;
	std::stack<unsigned int> loops;

	if (target == TargetPlatform::UNIX)
		nasm <<
		"global _start"			"\n"
		"section .bss"			"\n"
		"ptr: resq 1"			"\n"
		"memory: resb 1024"	"\n"
		"section .text"			"\n"
		"global _start"			"\n"
		"_start:"				"\n"
		"mov eax,0"				"\n"
		"mov [ptr],eax"			"\n";
	else if (target == TargetPlatform::WIN32NT)
		nasm <<
		//"extern AllocConsole"								"\n"
		"extern ExitProcess"							"\n"
		"extern GetStdHandle"							"\n"
		"extern ReadConsoleA"							"\n"
		"extern WriteConsoleA"							"\n"
		//"import AllocConsole kernel32.dll"				"\n"
		"import ExitProcess kernel32.dll"				"\n"
		"import GetStdHandle kernel32.dll"				"\n"
		"import ReadConsoleA kernel32.dll"				"\n"
		"import Sleep kernel32.dll"						"\n"
		"import WriteConsoleA kernel32.dll"				"\n"
		"section .bss"									"\n"
		"ptr: resq 1"									"\n"
		"memory: resb 1024"							"\n"
		"curr: resb 1"								"\n"
		"hStdOut: resd 1"								"\n"
		"hStdIn: resd 1"								"\n"
		"nBytes: resd 1"								"\n"
		"section .text use32"							"\n"
		"..start:"										"\n"
		"mov eax, 0"									"\n"
		"mov[ptr],eax"									"\n"
		//"; call[AllocConsole]; get da console!"			"\n"
		"push dword -11 ;stdout"						"\n"
		"call [GetStdHandle] ;handle is now in eax"		"\n"
		"mov [hStdOut],eax"								"\n"
		"push dword -10 ;stdin"							"\n"
		"call [GetStdHandle]"							"\n"
		"mov [hStdIn],eax"								"\n";

	for (char& c : bf) {
		switch (c) {
		case '>':
			nasm << "call _inc_ptr"		"\n";
			break;
		case '<':
			nasm << "call _dec_ptr"		"\n";
			break;
		case '+':
			nasm << "call _inc_at_ptr"	"\n";
			break;
		case '-':
			nasm << "call _dec_at_ptr"	"\n";
			break;
		case '.':
			nasm << "call _print_char"	"\n";
			break;
		case ',':
			nasm << "call _get_char"	"\n";
			break;
		case '[':
			loops.push(nextLoop);
			nasm <<
				"mov ecx,[ptr]"						"\n"
				"mov ebx,memory"					"\n"
				"mov al,[ebx + ecx]"				"\n"
				"cmp al,0"							"\n"
				"jz loop" << nextLoop << "_after"	"\n"
				"loop" << nextLoop << ":"			"\n";
			nextLoop++;
			break;
		case ']':
			unsigned int loop = loops.top();
			loops.pop();
			nasm <<
				"mov ecx,[ptr]"				"\n"
				"mov ebx,memory"			"\n"
				"mov al,[ebx + ecx]"		"\n"
				"cmp al,0"					"\n"
				"jnz loop" << loop <<		"\n"
				"loop" << loop << "_after:"	"\n";
			break;
		}
	}

	if (target == TargetPlatform::UNIX)
		nasm <<
		"mov eax,1"				"\n"
		"mov ebx,0"				"\n"
		"int 80h"				"\n"
		"_inc_ptr:"				"\n"
		"pusha"					"\n"
		"mov al,[ptr]"			"\n"
		"inc al"				"\n"
		"mov [ptr],al"			"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_dec_ptr:"				"\n"
		"pusha"					"\n"
		"mov al,[ptr]"			"\n"
		"dec al"				"\n"
		"mov [ptr],al"			"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_inc_at_ptr:"			"\n"
		"pusha"					"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"inc al"				"\n"
		"mov [ebx+ecx],al"		"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_dec_at_ptr:"			"\n"
		"pusha"					"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"dec al"				"\n"
		"mov [ebx+ecx],al"		"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_print_char:"			"\n"
		"pusha"					"\n"
		//";push esp"				"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"mov [esp],al"			"\n"
		"mov eax,4"				"\n"
		"mov ebx,1"				"\n"
		"mov ecx,esp"			"\n"
		"mov edx,1"				"\n"
		"int 80h"				"\n"
		//";pop esp"				"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_get_char:"			"\n"
		"pusha"					"\n"
		//";push esp"				"\n"
		"mov eax,3"				"\n"
		"mov ebx,0"				"\n"
		"mov ecx,esp"			"\n"
		"mov edx,1"				"\n"
		"int 80h"				"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[esp]"			"\n"
		"mov [ebx+ecx],al"		"\n"
		//";pop esp"				"\n"
		"popa"					"\n"
		"ret"					"\n";
	else if (target == TargetPlatform::WIN32NT)
		nasm <<
		"push dword 0"			"\n"
		"call [ExitProcess]"	"\n"
		"_inc_ptr:"				"\n"
		"pusha"					"\n"
		"mov al,[ptr]"			"\n"
		"inc al"				"\n"
		"mov [ptr],al"			"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_dec_ptr:"				"\n"
		"pusha"					"\n"
		"mov al,[ptr]"			"\n"
		"dec al"				"\n"
		"mov [ptr],al"			"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_inc_at_ptr:"			"\n"
		"pusha"					"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"inc al"				"\n"
		"mov [ebx+ecx],al"		"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_dec_at_ptr:"			"\n"
		"pusha"					"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"dec al"				"\n"
		"mov [ebx+ecx],al"		"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_print_char:"			"\n"
		"pusha"					"\n"
		//";push esp"				"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov al,[ebx+ecx]"		"\n"
		"mov [curr],al"			"\n"
		"push dword 0"			"\n"
		"push dword nBytes"		"\n"
		"push dword 1"			"\n"
		"push dword curr"		"\n"
		"push dword [hStdOut]"	"\n"
		"call [WriteConsoleA]"	"\n"
		//";pop esp"				"\n"
		"popa"					"\n"
		"ret"					"\n"
		"_get_char:"			"\n"
		"pusha"					"\n"
		//";push esp"				"\n"
		"push dword 0"			"\n"
		"push dword nBytes"		"\n"
		"push dword 1"			"\n"
		"push dword curr"		"\n"
		"push dword [hStdIn]"	"\n"
		"call [ReadConsoleA]"	"\n"
		"mov ecx,[ptr]"			"\n"
		"mov ebx,memory"		"\n"
		"mov eax,[curr]"		"\n"
		"mov [ebx+ecx],al"		"\n"
		//";pop esp"				"\n"
		"popa"					"\n"
		"ret"					"\n";
	return nasm.str();
}
