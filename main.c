#include "cpu.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void hook(int internal_offset, struct mm_zone* mm_zone, struct memory* memory)
{
	uint32_t value = memory
			     ->ptr[mm_zone->external_offset + internal_offset];
	printf("Memory mapped output: %d\n", value);
}

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
	memory_add_mm_zone(&cpu.memory, 500, 2, hook);
	fread(cpu.memory.ptr, 1000, 1, in);
	while (!cpu_is_halted(&cpu)) {
		cpu_tick(&cpu);
		usleep(100000);
		cpu_state_printdebug(&cpu);
	}
	cpu_free(&cpu);
	fclose(in);
	return 0;
}
