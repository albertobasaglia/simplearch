#ifndef CPU_H
#define CPU_H

#include "config.h"
#include "memory.h"
#include <stddef.h>
#include <stdint.h>

struct cpu_state {
	uint32_t r[GENERAL_REGISTER_COUNT];
	uint32_t sp;
	uint32_t pc;
#define CPU_FLAG_ZERO     (1 << 0)
#define CPU_FLAG_NEGATIVE (1 << 1)
#define CPU_FLAG_HALTED   (1 << 31)
	uint32_t flags;
};

int cpu_state_init(struct cpu_state* cpu_state);

struct cpu {
	struct memory memory;
	struct cpu_state state;
};

int cpu_init(struct cpu* cpu, int memory_size);

int cpu_free(struct cpu* cpu);

uint32_t cpu_fetch(struct cpu* cpu);

void cpu_exec(struct cpu* cpu, uint32_t instruction);

void cpu_tick(struct cpu* cpu);

void cpu_next(struct cpu* cpu);

int cpu_hard_load(struct cpu* cpu, size_t offset, uint32_t* ptr, size_t n);

void cpu_halt(struct cpu* cpu);

int cpu_is_halted(struct cpu* cpu);

void cpu_move_instruction(struct cpu* cpu, uint32_t instruction);

void cpu_memory_instruction(struct cpu* cpu, uint32_t instruction);

void cpu_arit_instruction(struct cpu* cpu, uint32_t instruction);

void cpu_branch_instruction(struct cpu* cpu, uint32_t instruction);

void cpu_state_printdebug(struct cpu* cpu);

void cpu_memory_hook(struct cpu* cpu, uint32_t address, int write);

#endif
