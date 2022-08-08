#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#include "config.h"
#include "label.h"

#define INSTRUCTION_MOVE     "MOV"
#define INSTRUCTION_ADD      "ADD"
#define INSTRUCTION_SUBTRACT "SUB"
#define INSTRUCTION_COMPARE  "CMP"
#define INSTRUCTION_BRANCH   "B"
#define INSTRUCTION_LOAD     "LOAD"
#define INSTRUCTION_STORE    "STORE"
#define INSTRUCTION_HALT     "HLT"

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
	int operand_count;
	enum instruction_operand_type operands[MAX_OPERANDS];
};

/*
 * Fills info with information about the current instruction.
 * Fills operands with their value (see function instruction_encode_operand).
 * Returns 0 if no labeling required. Returns 1 if labeling is required.
 * */
int instruction_populate_info(const char* str,
			      struct instruction_info* info,
			      uint32_t* operands,
			      char* label);

/*
 * Prints an instruction given its structure and the operands value.
 * */
void instruction_print_info(const struct instruction_info* info,
			    const uint32_t* operands_value);

static struct instruction_info instructions[] = {
    {
	.name = INSTRUCTION_MOVE,
	.operand_count = 2,
	.operands = {REGISTER, IMMEDIATE},
    },
    {
	.name = INSTRUCTION_MOVE,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_ADD,
	.operand_count = 3,
	.operands = {REGISTER, REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_SUBTRACT,
	.operand_count = 3,
	.operands = {REGISTER, REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_COMPARE,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_LOAD,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_STORE,
	.operand_count = 2,
	.operands = {REGISTER, REGISTER},
    },
    {
	.name = INSTRUCTION_HALT,
	.operand_count = 0,
	.operands = {},
    },
/* Branch instructions */
#define INSTRUCTION_B   "B"
#define INSTRUCTION_BEQ "BEQ"
#define INSTRUCTION_BNE "BNE"
#define INSTRUCTION_BGT "BGT"
#define INSTRUCTION_BGE "BGE"
    {
	.name = INSTRUCTION_BEQ,
	.operand_count = 1,
	.operands = {REGISTER, LABEL},
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

#define INSTRUCTION_SIGNATURE (0xf << 28)
/*
 * Encodes an instruction.
 * */
uint32_t instruction_encode(const char* str,
			    struct label** label_head_ptr,
			    uint32_t index);

#define INSTRUCTION_MEMORY_SIGNATURE      (0 << 28)
#define INSTRUCTION_MEMORY_STORE          (1 << 8)
#define INSTRUCTION_MEMORY_REGISTER_MASK  (0xf)
#define INSTRUCTION_MEMORY_REGISTER_SHIFT 0
#define INSTRUCTION_MEMORY_ADDRESS_MASK   (0xf)
#define INSTRUCTION_MEMORY_ADDRESS_SHIFT  4
/*
 * Encodes a memory instruction.
 *
 * [0,3] - register
 * [4,7] - address
 * [8]   - type
 * [28-31] = 0
 * */
uint32_t instruction_memory_encode(int reg, int address, int store);

#define INSTRUCTION_MOVE_SIGNATURE          (1 << 28)
#define INSTRUCTION_MOVE_IMM                (1 << 0)
#define INSTRUCTION_MOVE_SRC_SHIFT          1
#define INSTRUCTION_MOVE_SRC_MASK           0xf
#define INSTRUCTION_MOVE_IMM_SHIFT          1
#define INSTRUCTION_MOVE_IMM_MASK           0xffff // could be more
#define INSTRUCTION_MOVE_DEST_MASK          0xf
#define INSTRUCTION_MOVE_DEST_SHIFT         24
#define INSTRUCTION_MOVE_IMM_NEGATIVE_SHIFT (23)
#define INSTRUCTION_MOVE_IMM_NEGATIVE       (1 << INSTRUCTION_MOVE_IMM_NEGATIVE_SHIFT)
/*
 * Encodes a move instruction.
 *
 * [0] - 0(register)1/(immediate)
 * [24,27] - dest
 * [28-31] = 1
 *      [0] = 0:
 *        [1,4] - src
 *      [1] = 1:
 *        [1,22] - |value|
 *        [23]   - 0(positive)/1(negative)
 * */
uint32_t instruction_move_encode(int register_src, int register_dest);
uint32_t instruction_move_immediate_encode(int immediate, int register_dest);

#define INSTRUCTION_ARIT_SIGNATURE  (2 << 28)
#define INSTRUCTION_ARIT_DEST_SHIFT 0
#define INSTRUCTION_ARIT_DEST_MASK  0xf
#define INSTRUCTION_ARIT_OP1_SHIFT  4
#define INSTRUCTION_ARIT_OP1_MASK   0xf
#define INSTRUCTION_ARIT_OP2_SHIFT  8
#define INSTRUCTION_ARIT_OP2_MASK   0xf
#define INSTRUCTION_ARIT_TYPE_SHIFT 12
#define INSTRUCTION_ARIT_TYPE_MASK  0xf
#define INSTRUCTION_ARIT_TYPE_ADD   1
#define INSTRUCTION_ARIT_TYPE_SUB   2
#define INSTRUCTION_ARIT_TYPE_CMP   3
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
					int register_op2,
					int operation_type);

#define INSTRUCTION_BRANCH_SIGNATURE        (3 << 28)
#define INSTRUCTION_BRANCH_ADDRESS_SHIFT    0
#define INSTRUCTION_BRANCH_ADDRESS_MASK     0xffff
#define INSTRUCTION_BRANCH_ADDRESS_NEGATIVE (1 << 17)
#define INSTRUCTION_BRANCH_CONDITION_SHIFT  24
#define INSTRUCTION_BRANCH_CONDITION_MASK   0xf

#define INSTRUCTION_CONDITION_EQ            0
#define INSTRUCTION_CONDITION_NE            1
#define INSTRUCTION_CONDITION_GT            2
#define INSTRUCTION_CONDITION_GE            3
/*
 * Encodes a branch instruction.
 *
 * [0-16]  - |offset|
 * [17]    - 0(positive)/1(negative)
 * [24-27] - condition code
 * [28-31] = 3
 * */
uint32_t instruction_branch_encode(const char* label,
				   int condition_code,
				   struct label** head,
				   uint32_t index);

/*
 * Generates the address part of the branch instruction.
 * address - address where the branch is directed
 * index - address where the instruction will be placed
 * */
uint32_t instruction_branch_address_encode(uint32_t address, uint32_t index);

#define INSTRUCTION_HALT_SIGNATURE (4 << 28)
/*
 * Encodes an halt instruction.
 *
 * [28-31] = 4
 * */
uint32_t instruction_halt_encode();

#endif
