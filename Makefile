CC = gcc
OBJS = cpu.o \
       memory.o \
       instruction.o

all: emulator

emulator: main.c $(OBJS)
	$(CC) main.c $(OBJS) -o emulator

%.o : %.c %.h
	$(CC) -c $< -o $@

clean: emulator
	rm -f emulator $(OBJS)
