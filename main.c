#include "cpu.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Usage:\n"
		       "\temulator [binary file]\n");
		return -1;
	}
	FILE* in = fopen(argv[1], "rb");
	struct cpu cpu;
	cpu_init(&cpu, 0x1000);
	fread(cpu.memory.ptr, 1000, 1, in); // TODO do this better pls
	while (!cpu_is_halted(&cpu)) {
		cpu_tick(&cpu);
		usleep(1000000);
	}
	cpu_free(&cpu);
	fclose(in);
	return 0;
}
