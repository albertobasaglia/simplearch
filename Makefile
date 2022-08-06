CC = gcc -ggdb

OBJS = cpu.o \
       memory.o \
       instruction.o

all: emulator assembler

emulator: main.c $(OBJS)
	$(CC) main.c $(OBJS) -o emulator

assembler: assembler.c $(OBJS)
	$(CC) assembler.c $(OBJS) -o assembler

%.o : %.c %.h
	$(CC) -c $< -o $@

clean:
	rm -f emulator assembler $(OBJS)
