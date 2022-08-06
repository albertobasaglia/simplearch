#ifndef CPU_H
#define CPU_H

#include "config.h"
#include "memory.h"
#include <stdint.h>

struct cpu_state {
	uint32_t r[GENERAL_REGISTER_COUNT];
	uint32_t sp;
	uint32_t pc;
#define CPU_FLAG_ZERO (1 << 0)
	uint32_t flags;
};

int cpu_state_init(struct cpu_state* cpu_state);


struct cpu {
	struct memory memory;
	struct cpu_state state;
};

int cpu_init(struct cpu* cpu, int memory_size);

int cpu_free(struct cpu* cpu);

#endif
