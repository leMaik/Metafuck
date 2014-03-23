#include "BrainfuckNasmConverter.h"
#include <sstream>
#include <stack>

std::string bf2nasm(std::string bf)
{
	std::stringstream nasm;
	unsigned int nextLoop = 0;
	std::stack<unsigned int> loops;

	nasm
		<< "global _start" << std::endl
		<< "section .bss" << std::endl
		<< "ptr : resq	1" << std::endl
		<< "memory : resb	1024" << std::endl
		<< "section .text" << std::endl
		<< "global _start" << std::endl
		<< "_start:" << std::endl
		<< "mov eax,0" << std::endl
		<< "mov [ptr],eax" << std::endl;

	for (char& c : bf) {
		switch (c) {
		case '>':
			nasm << "call _inc_ptr" << std::endl;
			break;
		case '<':
			nasm << "call _dec_ptr" << std::endl;
			break;
		case '+':
			nasm << "call _inc_at_ptr" << std::endl;
			break;
		case '-':
			nasm << "call _dec_at_ptr" << std::endl;
			break;
		case '.':
			nasm << "call _print_char" << std::endl;
			break;
		case ',':
			nasm << "call _get_char" << std::endl;
			break;
		case '[':
			loops.push(nextLoop);
			nasm
				<< "mov ecx,[ptr]" << std::endl
				<< "mov ebx,memory" << std::endl
				<< "mov al,[ebx + ecx]" << std::endl
				<< "cmp al,0" << std::endl
				<< "jz loop" << nextLoop << "_after" << std::endl
				<< "loop" << nextLoop << ":" << std::endl;
			nextLoop++;
			break;
		case ']':
			unsigned int loop = loops.top();
			loops.pop();
			nasm
				<< "mov ecx,[ptr]" << std::endl
				<< "mov ebx,memory" << std::endl
				<< "mov al,[ebx + ecx]" << std::endl
				<< "cmp al,0" << std::endl
				<< "jnz loop" << loop << std::endl
				<< "loop" << loop << "_after" << std::endl;
			break;
		}
	}

	nasm << "\tmov eax,1\n\tmov ebx,0\t;exit code\n\tint 80h\n   \n_inc_ptr\n   pusha\n   mov al,[ptr]\n   inc al\n   mov [ptr],al\n   popa\n   ret\n   \n_dec_ptr\n   pusha\n   mov al,[ptr]\n   dec al\n   mov [ptr],al\n   popa\n   ret\n   \n_inc_at_ptr\n   pusha\n   mov ecx,[ptr]\n   mov ebx,memory\n   mov al,[ebx+ecx]\n   inc al\n   mov [ebx+ecx],al\n   popa\n   ret\n   \n_dec_at_ptr\n   pusha\n   mov ecx,[ptr]\n   mov ebx,memory\n   mov al,[ebx+ecx]\n   dec al\n   mov [ebx+ecx],al\n   popa\n   ret\n   \n_print_char:\n   pusha\n   ;push esp\n   \n\tmov ecx,[ptr]\n   mov ebx,memory\n   mov al,[ebx+ecx]\n   mov [esp],al\n   \n\tmov eax,4\n\tmov ebx,1 \t\t; stdout\n   mov ecx,esp\n\tmov edx,1\n\tint 80h\n   \n   ;pop esp\n   popa\n   ret\n   \n_get_char:\n   pusha\n   ;push esp\n   \n\tmov eax,3\n\tmov ebx,0 \t\t; stdin\n   mov ecx,esp\n\tmov edx,1\n\tint 80h\n   \n\tmov ecx,[ptr]\n   mov ebx,memory\n   mov al,[esp]\n   mov [ebx+ecx],al\n   \n   ;pop esp\n   popa\n   ret";
	return nasm.str();
}
