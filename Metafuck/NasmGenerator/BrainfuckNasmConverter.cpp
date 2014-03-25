#include "BrainfuckNasmConverter.h"
#include <sstream>
#include <stack>
#include <vector>
#include <memory>

std::string bf2nasm(std::string bf, TargetPlatform target)
{
	std::stringstream nasm;
	unsigned int nextLoop = 0;

	if (target == TargetPlatform::UNIX)
		nasm <<
		"global _start"			"\n"
		"section .bss"			"\n"
		"ptr: resq 1"			"\n"
		"memory: resb 1024"		"\n"
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
		"memory: resb 1024"								"\n"
		"curr: resb 1"									"\n"
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

	std::stack<BrainfuckInstructions> instructions;
	instructions.push(BrainfuckInstructions());
	BrainfuckInstruction::Type currentType = BrainfuckInstruction::Type::NONE;

	unsigned int length = bf.length();
	unsigned int i = 0;
	while (i < length) {
		auto c = bf.at(i);
		switch (c) {
		case '>':
			if (currentType != BrainfuckInstruction::Type::MOVE_POINTER){
				instructions.top().push_back(std::shared_ptr<MovePointer>(new MovePointer));
				currentType = BrainfuckInstruction::Type::MOVE_POINTER;
			}
			static_cast<MovePointer&>(*instructions.top().back()).inc();
			break;
		case '<':
			if (currentType != BrainfuckInstruction::Type::MOVE_POINTER){
				instructions.top().push_back(std::shared_ptr<MovePointer>(new MovePointer));
				currentType = BrainfuckInstruction::Type::MOVE_POINTER;
			}
			static_cast<MovePointer&>(*instructions.top().back()).dec();
			break;
		case '+':
			if (currentType != BrainfuckInstruction::Type::CHANGE_VALUE){
				instructions.top().push_back(std::shared_ptr<ChangeValue>());
				currentType = BrainfuckInstruction::Type::CHANGE_VALUE;
			}
			static_cast<ChangeValue&>(*instructions.top().back()).inc();
			break;
		case '-':
			if (currentType != BrainfuckInstruction::Type::CHANGE_VALUE){
				instructions.top().push_back(std::shared_ptr<ChangeValue>(new ChangeValue));
				currentType = BrainfuckInstruction::Type::CHANGE_VALUE;
			}
			static_cast<ChangeValue&>(*instructions.top().back()).dec();
			break;
		case '.':
			instructions.top().push_back(std::shared_ptr<PrintChar>(new PrintChar));
			currentType = BrainfuckInstruction::Type::NONE;
			break;
		case ',':
			instructions.top().push_back(std::shared_ptr<GetChar>(new GetChar));
			currentType = BrainfuckInstruction::Type::NONE;
			break;
		case '[':
			if (i + 2 < length && bf.at(i + 1) == '-' && bf.at(i + 2) == ']') {
				i += 2;
				instructions.top().push_back(std::shared_ptr<ClearValue>(new ClearValue));
				currentType = BrainfuckInstruction::Type::NONE;
			}
			else {
				instructions.push(BrainfuckInstructions());
				currentType = BrainfuckInstruction::Type::NONE;
			}
			break;
		case ']':
			auto loopContent = instructions.top();
			instructions.pop();
			instructions.top().push_back(std::shared_ptr<Loop>(new Loop(nextLoop++, loopContent)));
			currentType = BrainfuckInstruction::Type::NONE;
			/*case '[':
				if (i + 2 < length && bf.at(i + 1) == '-' && bf.at(i + 2) == ']') {
				i += 2;
				nasm <<
				"mov ecx,[ptr]"						"\n"
				"mov ebx,memory"					"\n"
				"mov al,0"							"\n"
				"mov [ebx + ecx],al"				"\n";
				}
				else {
				loops.push(nextLoop);
				nasm <<
				"mov ecx,[ptr]"						"\n"
				"mov ebx,memory"					"\n"
				"mov al,[ebx + ecx]"				"\n"
				"cmp al,0"							"\n"
				"jz loop" << nextLoop << "_after"	"\n"
				"loop" << nextLoop << ":"			"\n";
				nextLoop++;
				}
				break;
				case ']':
				unsigned int loop = loops.top();
				loops.pop();
				nasm <<
				"mov ecx,[ptr]"				"\n"
				"mov ebx,memory"			"\n"
				"mov al,[ebx + ecx]"		"\n"
				"cmp al,0"					"\n"
				"jnz loop" << loop << "\n"
				"loop" << loop << "_after:"	"\n";
				break;*/
		}

		i++;
	}

	for (auto& instruction : instructions.top())
		nasm << instruction->toNasm();

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

void ChangeValue::inc(){
	count++;
}

void ChangeValue::dec(){
	count--;
}

std::string ChangeValue::toNasm(){
	if (count == 0)
		return "";
	if (count > 0){
		if (count == 1) {
			return
				"mov ecx,[ptr]"			"\n"
				"mov ebx,memory"		"\n"
				"mov al,[ebx+ecx]"		"\n"
				"inc al"				"\n"
				"mov [ebx+ecx],al"		"\n";
		}
		else {
			return
				"mov ecx,[ptr]"			"\n"
				"mov ebx,memory"		"\n"
				"mov al,[ebx+ecx]"		"\n"
				"add al," + std::to_string(count) + "\n"
				"mov [ebx+ecx],al"		"\n";
		}
	}
	else {
		if (count == -1) {
			return
				"mov ecx,[ptr]"			"\n"
				"mov ebx,memory"		"\n"
				"mov al,[ebx+ecx]"		"\n"
				"dec al"				"\n"
				"mov [ebx+ecx],al"		"\n";
		}
		else {
			return
				"mov ecx,[ptr]"			"\n"
				"mov ebx,memory"		"\n"
				"mov al,[ebx+ecx]"		"\n"
				"sub al," + std::to_string(-count) + "\n"
				"mov [ebx+ecx],al"		"\n";
		}
	}
}

void MovePointer::inc(){
	count++;
}

void MovePointer::dec(){
	count--;
}

std::string MovePointer::toNasm(){
	if (count == 0)
		return "";
	if (count > 0){
		if (count == 1) {
			return
				"mov al,[ptr]"			"\n"
				"inc al"				"\n"
				"mov [ptr],al"			"\n";
		}
		else {
			return
				"mov al,[ptr]"			"\n"
				"add al," + std::to_string(count) + "\n"
				"mov [ptr],al"			"\n";
		}
	}
	else {
		if (count == -1) {
			return
				"mov al,[ptr]"			"\n"
				"dec al"				"\n"
				"mov [ptr],al"			"\n";
		}
		else {
			return
				"mov al,[ptr]"			"\n"
				"sub al," + std::to_string(-count) + "\n"
				"mov [ptr],al"			"\n";
		}
	}
}

std::string ClearValue::toNasm(){
	return
		"mov ecx, [ptr]"		"\n"
		"mov ebx, memory"		"\n"
		"mov al, 0"				"\n"
		"mov[ebx + ecx], al"	"\n";
}