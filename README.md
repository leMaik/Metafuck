Metafuck
========

Metafuck is a compiler that translates an awesome programming language to Brainfuck and it also includes an interpreter that runs the generated code. It can also translate brainfuck code to assembler (NASM syntax), with either *nix or Windows support (using system calls or Win32 API).

An additional program that just takes brainfuck and outputs an assembly is coming soon.

How to compile
--------------

Metafuck will always compile lovely with the Visual C++ Compiler. Not that we love it but the IDE is pretty nice. Our goal is to make Metafuck compile with g++ and we will make sure that it does frequently. Still some commits may not compile with g++.

Boost is required by Metafuck (i.e. for the command line options), so you should get that first if you don't already have it installed.

To compile, you can simply use the makefile provided, simply specify the boost path before you run it.

How to use
----------

Compile a metafuck file "test.mf" to brainfuck and run it: `metafuck -ro test.bf test.mf` (`Metafuck.exe` on Windows)

Compile a metafuck file "test.mf" to a NASM assembly (and try to assembler it): `metafuck -n unix test.mf` (for *nix) or `Metafuck.exe -n win32 test.mf` (for Windows)
