#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "config.h"
#include <stdint.h>

#define INSTRUCTION_MOVE     "MOV"
#define INSTRUCTION_ADD      "ADD"
#define INSTRUCTION_SUBTRACT "SUB"
#define INSTRUCTION_COMPARE  "CMP"
#define INSTRUCTION_BRANCH   "B"
#define INSTRUCTION_LOAD     "LOAD"
#define INSTRUCTION_STORE    "STORE"

enum instruction_operand_type {
	IMMEDIATE,
	REGISTER,
	LABEL,
};

enum instruction_operand_type instruction_decode_operand(const char* str);

/*
 * Returns information about the operand.
 * - IMMEDIATE: returns its value
 * - REGISTER: resturns register id
 * */
uint32_t instruction_getvalue_operand(const char* str,
				      enum instruction_operand_type type);

struct instruction_info {
	char name[10];
	int conditionable;
	int operand_count;
	enum instruction_operand_type operands[MAX_OPERANDS];
};

/*
 * Fills info with information about the current instruction.
 * Fills operands with their value (see function instruction_encode_operand).
 * */
int instruction_populate_info(const char* str,
			      struct instruction_info* info,
			      uint32_t* operands);

/*
 * Prints an instruction given its structure and the operands value.
 * */
void instruction_print_info(const struct instruction_info* info,
			    const uint32_t* operands_value);

static struct instruction_info instructions[] = {
    {
	.name = INSTRUCTION_MOVE,
	.conditionable = 0,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_ADD,
	.conditionable = 0,
	.operand_count = 3,
	.operands = {REGISTER, REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_ADD,
	.conditionable = 0,
	.operand_count = 3,
	.operands = {REGISTER, REGISTER, IMMEDIATE},
    },
    {
	.name = INSTRUCTION_SUBTRACT,
	.conditionable = 0,
	.operand_count = 3,
	.operands = {REGISTER, REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_COMPARE,
	.conditionable = 0,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_COMPARE,
	.conditionable = 0,
	.operand_count = 2,
	.operands = {REGISTER, IMMEDIATE},
    },
    {
	.name = INSTRUCTION_BRANCH,
	.conditionable = 1,
	.operand_count = 1,
	.operands = {REGISTER, LABEL},
    },
    {
	.name = INSTRUCTION_LOAD,
	.conditionable = 0,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_STORE,
	.conditionable = 0,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
};

/*
 * Fills infos with instruction descriptors that match the name
 * */
int instruction_match_name(const char* name, struct instruction_info* infos);

/*
 * Looks for a descriptor that matches the given one.
 * If it matches, returns 1. Returns 0 otherwise.
 * */
int instruction_match_all(const char* name,
			  const struct instruction_info* infos);

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
uint32_t instruction_encode(const char* str);

/*
 * Encodes a memory instruction.
 *
 * [0,3] - src
 * [4,7] - dest
 * [8]   - type
 * [28-31] = 0
 * */
uint32_t instruction_memory_encode(int register_src,
				   int register_dest,
				   int operation_type);

/*
 * Encodes a move instruction.
 *
 * [0,3] - src
 * [4,7] - dest
 * [28-31] = 1
 * */
uint32_t instruction_move_encode(int register_src, int register_dest);

/*
 * Encodes an arithmetic instruction.
 *
 * [0,3]   - dest
 * [4,7]   - op1
 * [8-11]  - op2
 * [12-15] - type
 * [28-31] = 2
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
 * [16-27] - condition code
 * [28-31] = 3
 * */
uint32_t instruction_branch_encode(int address_offset, int condition_code);

#endif
