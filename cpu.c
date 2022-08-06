#include "cpu.h"
#include "config.h"
#include "instruction.h"
#include "memory.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

uint32_t cpu_fetch(struct cpu* cpu)
{
	return cpu->memory.ptr[cpu->state.pc];
}

void cpu_exec(struct cpu* cpu, uint32_t instruction)
{
	printf("Executing %#x\n", instruction);
	uint32_t signature = instruction & INSTRUCTION_SIGNATURE;
	if (signature == INSTRUCTION_MOVE_SIGNATURE) {
		printf("Move signature\n");
	} else if (signature == INSTRUCTION_MEMORY_SIGNATURE) {
		printf("Memory signature\n");
	} else if (signature == INSTRUCTION_HALT_SIGNATURE) {
                printf("Halt signature\n");
                cpu_halt(cpu);
	}
}

void cpu_tick(struct cpu* cpu)
{
	uint32_t fetch = cpu_fetch(cpu);
	cpu_exec(cpu, fetch);
	cpu_next(cpu);
}

void cpu_next(struct cpu* cpu)
{
	cpu->state.pc++;
}

int cpu_hard_load(struct cpu* cpu, size_t offset, uint32_t* ptr, size_t n)
{
	if (offset + n >= cpu->memory.size)
		return -1;
	memcpy(cpu->memory.ptr + offset, ptr, n * sizeof(uint32_t));
	return 0;
}

void cpu_halt(struct cpu* cpu)
{
	cpu->state.flags |= CPU_FLAG_HALTED;
}

int cpu_is_halted(struct cpu* cpu)
{
	return cpu->state.flags & CPU_FLAG_HALTED;
}
