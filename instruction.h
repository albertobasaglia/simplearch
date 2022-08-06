#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

enum instruction_type {
	MEMORY, // instructions for moving data form/to main memory
#define INSTRUCTION_MEMORY_STORE 0
#define INSTRUCTION_MEMORY_LOAD  1

	MOVE, // move data between registers

	ARITHMETICS,                // arithmetic operations (and compare)
#define INSTRUCTION_ARITHMETICS_ADD // encodes both ADD and SUB
#define INSTRUCTION_ARITHMETICS_CMP // compare

	BRANCH, // relative address branch
#define INSTRUCTION_BRANCH_CONDITION_EQ 1
#define INSTRUCTION_BRANCH_CONDITION_NE 2
#define INSTRUCTION_BRANCH_CONDITION_GT 3
#define INSTRUCTION_BRANCH_CONDITION_GE 4
};

/*
 * Encodes an instruction.
 * */
uint32_t instruction_encode(enum instruction_type type, const char* str);

/*
 * Encodes a memory instruction.
 *
 * [0,3] - src
 * [4,7] - dest
 * [8]   - type
 * */
uint32_t instruction_memory_encode(int register_src,
				   int register_dest,
				   int operation_type);

/*
 * Encodes a move instruction.
 *
 * [0,3] - src
 * [4,7] - dest
 * */
uint32_t instruction_move_encode(int register_src, int register_dest);

/*
 * Encodes an arithmetic instruction.
 *
 * [0,3]   - dest
 * [4,7]   - op1
 * [8-11]  - op2
 * [12-15] - type
 * */
uint32_t instruction_arithmetics_encode(int register_dest,
					int register_op1,
					int resgister_op2,
					int operation_type);

/*
 * Encodes a branch instruction.
 *
 * [0-14]  - |offset|
 * [15] - 0(positive)/1(negative)
 * [16-31] - condition code
 * */
uint32_t instruction_branch_encode(int address_offset, int condition_code);

#endif
