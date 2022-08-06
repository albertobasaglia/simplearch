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
	/* printf("Executing %#x\n", instruction); */
	uint32_t signature = instruction & INSTRUCTION_SIGNATURE;
	if (signature == INSTRUCTION_MOVE_SIGNATURE) {
		cpu_move_instruction(cpu, instruction);
	} else if (signature == INSTRUCTION_MEMORY_SIGNATURE) {
		cpu_memory_instruction(cpu, instruction);
	} else if (signature == INSTRUCTION_HALT_SIGNATURE) {
		cpu_halt(cpu);
	} else if (signature == INSTRUCTION_ARIT_SIGNATURE) {
		cpu_arit_instruction(cpu, instruction);
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

void cpu_move_instruction(struct cpu* cpu, uint32_t instruction)
{
	if (instruction & INSTRUCTION_MOVE_IMM) {
		uint32_t src;
		uint32_t dest;
		dest = (instruction >> INSTRUCTION_MOVE_DEST_SHIFT) &
		       INSTRUCTION_MOVE_DEST_MASK;
		src = (instruction >> INSTRUCTION_MOVE_IMM_SHIFT) &
		      INSTRUCTION_MOVE_IMM_MASK;
		if ((instruction >> INSTRUCTION_MOVE_IMM_NEGATIVE_SHIFT) & 1) {
			src *= -1;
		}
		cpu->state.r[dest] = src;
	} else {
		uint32_t src;
		uint32_t dest;
		src = (instruction >> INSTRUCTION_MOVE_SRC_SHIFT) &
		      INSTRUCTION_MOVE_SRC_MASK;
		dest = (instruction >> INSTRUCTION_MOVE_DEST_SHIFT) &
		       INSTRUCTION_MOVE_DEST_MASK;
		cpu->state.r[dest] = cpu->state.r[src];
	}
}

void cpu_memory_instruction(struct cpu* cpu, uint32_t instruction)
{
	uint32_t reg;
	uint32_t address;
	reg = (instruction >> INSTRUCTION_MEMORY_REGISTER_SHIFT) &
	      INSTRUCTION_MEMORY_REGISTER_MASK;
	address = (instruction >> INSTRUCTION_MEMORY_ADDRESS_SHIFT) &
		  INSTRUCTION_MEMORY_ADDRESS_MASK;
	uint32_t address_value = cpu->state.r[address];
	if (instruction & INSTRUCTION_MEMORY_STORE) {
		memory_write(&cpu->memory, address_value, cpu->state.r[reg]);
		cpu_memory_hook(cpu, address_value, 1);
	} else {
		uint32_t read;
		memory_read(&cpu->memory, address_value, &read);
		cpu->state.r[reg] = read;
		cpu_memory_hook(cpu, address_value, 0);
	}
}

void cpu_arit_instruction(struct cpu* cpu, uint32_t instruction)
{
	uint32_t reg_dest;
	uint32_t reg_op1;
	uint32_t reg_op2;

	uint32_t type = (instruction >> INSTRUCTION_ARIT_TYPE_SHIFT) &
			INSTRUCTION_ARIT_TYPE_MASK;

	reg_op1 = (instruction >> INSTRUCTION_ARIT_OP1_SHIFT) &
		  INSTRUCTION_ARIT_OP1_MASK;
	reg_op2 = (instruction >> INSTRUCTION_ARIT_OP2_SHIFT) &
		  INSTRUCTION_ARIT_OP2_MASK;

	int val1 = cpu->state.r[reg_op1];
	int val2 = cpu->state.r[reg_op2];

	if (type != INSTRUCTION_ARIT_TYPE_SUB) {
		reg_dest = (instruction >> INSTRUCTION_ARIT_DEST_SHIFT) &
			   INSTRUCTION_ARIT_DEST_MASK;

		if (type == INSTRUCTION_ARIT_TYPE_SUB)
			val2 *= -1;

		cpu->state.r[reg_dest] = val1 + val2;
	} else {
		int res = val1 - val2;
		if (res == 0) {
			cpu->state.flags |= CPU_FLAG_ZERO;
		} else if (res < 0) {
			cpu->state.flags |= CPU_FLAG_NEGATIVE;
		}
	}
}

void cpu_memory_hook(struct cpu* cpu, uint32_t address, int write)
{
	struct mm_zone* ptr = cpu->memory.mm_zone_head;
	while (ptr != NULL) {
		if (address >= ptr->external_offset &&
		    address < ptr->external_offset + ptr->size) {
			ptr->hook(address - ptr->external_offset, ptr,
				  &cpu->memory);
		}
		ptr = ptr->next;
	}
}

void cpu_state_printdebug(struct cpu* cpu)
{
	printf("Registers: ");
	for (int rc = 0; rc < GENERAL_REGISTER_COUNT; rc++) {
		if (rc % 4 == 0)
			printf("\n");
		printf("R%d: %08x\t", rc, cpu->state.r[rc]);
	}
	printf("\nPC: %0#x\t", cpu->state.pc);
	printf("FLAGS: %0#x\n", cpu->state.flags);
}
