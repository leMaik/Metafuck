BOOST_PATH = /home/maik/boost/boost_1_55_0/

CC = g++
CFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -static -lboost_program_options -L$(BOOST_PATH)stage/lib/
OUTPUT = mf

all: mf

mf: Argument.o brainfuck.o Call.o CallList.o compiler.o helper.o main.o Number.o String.o Variable.o ConvenienceFuck.o PrecomFuck.o BrainfuckNasmConverter.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) -isystem$(BOOST_PATH) $<

Argument.o: Argument.cpp Argument.h
	$(CC) -c $(CFLAGS) $<

brainfuck.o: brainfuck.cpp brainfuck.h
	$(CC) -c $(CFLAGS) $<

Call.o: Call.cpp Call.h
	$(CC) -c $(CFLAGS) $<

CallList.o: CallList.cpp CallList.h
	$(CC) -c $(CFLAGS) $<

compiler.o: compiler.cpp compiler.h
	$(CC) -c $(CFLAGS) $<

helper.o: helper.cpp helper.h
	$(CC) -c $(CFLAGS) $<

Number.o: Number.cpp Number.h
	$(CC) -c $(CFLAGS) $<

String.o: String.cpp String.h
	$(CC) -c $(CFLAGS) $<

Variable.o: Variable.cpp Variable.h
	$(CC) -c $(CFLAGS) $<

ConvenienceFuck.o: Interpreter/ConvenienceFuck.cpp Interpreter/ConvenienceFuck.h
	$(CC) -c $(CFLAGS) $<

PrecomFuck.o: Interpreter/PrecomFuck.cpp Interpreter/PrecomFuck.h
	$(CC) -c $(CFLAGS) $<

BrainfuckNasmConverter.o: NasmGenerator/BrainfuckNasmConverter.cpp NasmGenerator/BrainfuckNasmConverter.h
	$(CC) -c $(CFLAGS) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm mf
