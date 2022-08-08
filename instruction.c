#include "instruction.h"
#include "config.h"
#include "label.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum instruction_operand_type instruction_decode_operand(const char* str)
{
	// TODO handle wrong format
	if (str[0] == 'R')
		return REGISTER;
	if (str[0] == '#')
		return IMMEDIATE;
	return LABEL;
}

int instruction_match_name(const char* name, struct instruction_info* infos)
{
	int inserted = 0;
	int instruction_info_count = sizeof(instructions) /
				     sizeof(struct instruction_info);
	for (int i = 0; i < instruction_info_count; i++) {
		if (strcmp(name, instructions[i].name) == 0) {
			infos[inserted] = instructions[i];
			inserted++;
		}
	}
	return inserted;
}

int instruction_match_all(const char* name,
			  const struct instruction_info* infos)
{
	struct instruction_info possible[MAX_OVERLOADS];
	int count = instruction_match_name(name, possible);

	for (int i = 0; i < count; i++) {
		if (infos->operand_count != possible[i].operand_count)
			continue;
		int valid = 1;
		for (int o = 0; o < infos->operand_count; o++) {
			if (infos->operands[o] != possible[i].operands[o]) {
				valid = 0;
				break;
			}
		}
		if (!valid)
			continue;
		return 1;
	}
	return 0;
}

uint32_t instruction_getvalue_operand(const char* str,
				      enum instruction_operand_type type)
{
	if (type == IMMEDIATE) {
		uint32_t res;
		sscanf(str, "#%d", &res);
		return res;
	}
	if (type == REGISTER) {
		uint32_t res;
		sscanf(str, "R%d", &res);
		return res;
	}
	return 0;
}

void instruction_print_info(const struct instruction_info* info,
			    const uint32_t* operands_value)
{
	printf("Name: %s\nOperands count: %d\n", info->name,
	       info->operand_count);
	for (int i = 0; i < info->operand_count; i++) {
		enum instruction_operand_type type = info->operands[i];
		uint32_t value = operands_value[i];
		printf("%d -> ", i + 1);
		switch (type) {
		case IMMEDIATE:
			printf("#%d\n", value);
			break;
		case REGISTER:
			printf("r%d\n", value);
			break;
		case LABEL:
			break;
		}
	}
}

int instruction_populate_info(const char* str,
			      struct instruction_info* info,
			      uint32_t* operands,
			      char* label)
{
	int labeling = 0;
	char instruction[LINEBUFFER_SIZE];
	strncpy(instruction, str, LINEBUFFER_SIZE);

	char* name = strtok(instruction, VALID_SPLIT_TOKEN);

	enum instruction_operand_type operands_type[MAX_OPERANDS];
	int index = 0;

	char* operand = strtok(NULL, VALID_SPLIT_TOKEN);
	while (operand != NULL) {
		operands_type[index] = instruction_decode_operand(operand);
		if (operands_type[index] != LABEL) {
			operands[index] = instruction_getvalue_operand(
			    operand, operands_type[index]);
		} else {
			labeling = 1;
			strncpy(label, operand, LABEL_MAX_LENGTH);
		}
		operand = strtok(NULL, VALID_SPLIT_TOKEN);
		index++;
	}

	info->operand_count = index;
	strncpy(info->name, name, 10);
	memcpy(info->operands, operands_type,
	       index * sizeof(enum instruction_operand_type));

	if (!instruction_match_all(info->name, info))
		return ERR_INVALIDINSTRUCTION;

	return labeling;
}

uint32_t instruction_move_encode(int register_src, int register_dest)
{
	uint32_t instruction = 0;

	instruction |= INSTRUCTION_MOVE_SIGNATURE;
	register_src &= INSTRUCTION_MOVE_SRC_MASK;

	instruction |= (register_src << INSTRUCTION_MOVE_SRC_SHIFT);
	register_dest &= INSTRUCTION_MOVE_DEST_MASK;

	instruction |= (register_dest << INSTRUCTION_MOVE_DEST_SHIFT);

	return instruction;
}

uint32_t instruction_move_immediate_encode(int immediate, int register_dest)
{
	uint32_t instruction = 0;
	instruction |= INSTRUCTION_MOVE_SIGNATURE;
	instruction |= INSTRUCTION_MOVE_IMM;

	int positive = immediate >= 0;
	if (!positive)
		immediate = -immediate;

	immediate &= INSTRUCTION_MOVE_IMM_MASK;
	instruction |= (immediate << INSTRUCTION_MOVE_IMM_SHIFT);

	register_dest &= INSTRUCTION_MOVE_SRC_MASK;
	instruction |= (register_dest << INSTRUCTION_MOVE_DEST_SHIFT);

	if (!positive)
		instruction |= INSTRUCTION_MOVE_IMM_NEGATIVE;

	return instruction;
}

uint32_t instruction_memory_encode(int reg, int address, int store)
{
	uint32_t instruction = 0;
	instruction |= INSTRUCTION_MEMORY_SIGNATURE;

	if (store)
		instruction |= INSTRUCTION_MEMORY_STORE;

	reg &= INSTRUCTION_MEMORY_REGISTER_MASK;

	address &= INSTRUCTION_MEMORY_ADDRESS_MASK;

	instruction |= (reg << INSTRUCTION_MEMORY_REGISTER_SHIFT);
	instruction |= (address << INSTRUCTION_MEMORY_ADDRESS_SHIFT);

	return instruction;
}

uint32_t instruction_halt_encode()
{
	return INSTRUCTION_HALT_SIGNATURE;
}

uint32_t instruction_arithmetics_encode(int register_dest,
					int register_op1,
					int register_op2,
					int operation_type)
{
	uint32_t instruction = 0;

	instruction |= INSTRUCTION_ARIT_SIGNATURE;
	if (operation_type != INSTRUCTION_ARIT_TYPE_CMP) {
		instruction |= (register_dest & INSTRUCTION_ARIT_DEST_MASK)
			       << INSTRUCTION_ARIT_DEST_SHIFT;
	}

	instruction |= (register_op1 & INSTRUCTION_ARIT_OP1_MASK)
		       << INSTRUCTION_ARIT_OP1_SHIFT;

	instruction |= (register_op2 & INSTRUCTION_ARIT_OP2_MASK)
		       << INSTRUCTION_ARIT_OP2_SHIFT;

	instruction |= operation_type << INSTRUCTION_ARIT_TYPE_SHIFT;

	return instruction;
}

uint32_t instruction_branch_address_encode(uint32_t address, uint32_t index)
{
	uint32_t format = 0;
	int addressi = address;
	int indexi = index;
	int offset = address - index;
	// TODO check offset size not exceeding constraints
	int negative = offset < 0 ? 1 : 0;
	if (negative) {
		offset *= -1;
		format |= INSTRUCTION_BRANCH_ADDRESS_NEGATIVE;
	}
	format |= (offset & INSTRUCTION_BRANCH_ADDRESS_MASK)
		  << INSTRUCTION_BRANCH_ADDRESS_SHIFT;
	return format;
}

uint32_t instruction_branch_encode(const char* label,
				   int condition_code,
				   struct label** head,
				   uint32_t index)
{
	uint32_t instruction = 0;
	instruction |= INSTRUCTION_BRANCH_SIGNATURE;
	instruction |= (condition_code & INSTRUCTION_BRANCH_CONDITION_MASK)
		       << INSTRUCTION_BRANCH_CONDITION_SHIFT;
	if (*head == NULL) {
		printf("No list to search for symbol\n");
		struct label* created = label_new_label(head, label, 0, 0);
	} else {
		struct label* existing = label_find_name(*head, label);
		if (existing == NULL) {
			label_new_label(head, label, 0, 0);
			printf("Symbol not in the list\n");
		} else {
			printf("Symbol found with address: %d\n",
			       existing->value);
			instruction |= instruction_branch_address_encode(
			    existing->value, index);
		}
	}
	return instruction;
}

uint32_t instruction_encode(const char* str,
			    struct label** label_head_ptr,
			    uint32_t index)
{
	struct instruction_info info;
	uint32_t operands[MAX_OPERANDS];
	char label_if_labeling[LABEL_MAX_LENGTH];
	int labeling;
	labeling = instruction_populate_info(str, &info, operands,
					     label_if_labeling);
	instruction_print_info(&info, operands);

	if (strcmp(info.name, INSTRUCTION_MOVE) == 0) {
		if (info.operands[1] == IMMEDIATE)
			return instruction_move_immediate_encode(operands[1],
								 operands[0]);
		return instruction_move_encode(operands[1], operands[0]);
	} else if (strcmp(info.name, INSTRUCTION_STORE) == 0) {
		return instruction_memory_encode(operands[0], operands[1], 1);
	} else if (strcmp(info.name, INSTRUCTION_LOAD) == 0) {
		return instruction_memory_encode(operands[0], operands[1], 0);
	} else if (strcmp(info.name, INSTRUCTION_HALT) == 0) {
		return instruction_halt_encode();
	} else if (strcmp(info.name, INSTRUCTION_ADD) == 0) {
		return instruction_arithmetics_encode(
		    operands[0], operands[1], operands[2],
		    INSTRUCTION_ARIT_TYPE_ADD);
	} else if (strcmp(info.name, INSTRUCTION_SUBTRACT) == 0) {
		return instruction_arithmetics_encode(
		    operands[0], operands[1], operands[2],
		    INSTRUCTION_ARIT_TYPE_SUB);
	} else if (strcmp(info.name, INSTRUCTION_COMPARE) == 0) {
		return instruction_arithmetics_encode(
		    -1, operands[0], operands[1], INSTRUCTION_ARIT_TYPE_CMP);
	} else if (labeling && strcmp(info.name, INSTRUCTION_BEQ) == 0) {
		return instruction_branch_encode(label_if_labeling,
						 INSTRUCTION_CONDITION_EQ,
						 label_head_ptr, index);
	} else if (labeling && strcmp(info.name, INSTRUCTION_BNE) == 0) {
		return instruction_branch_encode(label_if_labeling,
						 INSTRUCTION_CONDITION_NE,
						 label_head_ptr, index);
	}
	printf("Undefined instruction!\n");

	return 0;
}
