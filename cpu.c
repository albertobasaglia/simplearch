#include "cpu.h"
#include "config.h"
#include "memory.h"
#include <stdlib.h>

int cpu_state_init(struct cpu_state* cpu_state)
{
	for (int rn = 0; rn < GENERAL_REGISTER_COUNT; rn++) {
		cpu_state->r[rn] = 0;
	}
	cpu_state->sp = 0;
	cpu_state->pc = 0;
	cpu_state->flags = 0;
	return 0;
}

int cpu_init(struct cpu* cpu, int memory_size)
{
	cpu_state_init(&cpu->state);
	int res = memory_init(&cpu->memory, memory_size);
	if (res != 0)
		return res;
	return 0;
}

int cpu_free(struct cpu* cpu)
{
	memory_free(&cpu->memory);
	return 0;
}
