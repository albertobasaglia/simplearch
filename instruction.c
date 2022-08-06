#include "instruction.h"
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum instruction_operand_type instruction_decode_operand(const char* str)
{
	// TODO handle wrong format
	if (str[0] == 'R')
		return REGISTER;
	if (str[0] == '=')
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
		if (infos->conditionable != possible[i].conditionable)
			continue;
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

uint32_t instruction_encode_operand(const char* str,
				    enum instruction_operand_type type)
{
	if (type == IMMEDIATE) {
		uint32_t res;
		sscanf(str, "=%d", &res);
		return res;
	}
	if (type == REGISTER) {
		uint32_t res;
		sscanf(str, "r%d", &res);
		return res;
	}
	return 0;
}

int instruction_populate_info(const char* str, struct instruction_info* info)
{
	char instruction[LINEBUFFER_SIZE];
	strncpy(instruction, str, LINEBUFFER_SIZE);

	char* name = strtok(instruction, " ,");

	enum instruction_operand_type operands[MAX_OPERANDS];
	int index = 0;

	char* operand = strtok(NULL, " ,");
	while (operand != NULL) {
		operands[index] = instruction_decode_operand(operand);
		operand = strtok(NULL, " ,");
		index++;
	}

	info->conditionable = 0; // TODO
	info->operand_count = index;
	strncpy(info->name, name, 10);
	memcpy(info->operands, operands,
	       index * sizeof(enum instruction_operand_type));

	if (!instruction_match_all(info->name, info))
		return ERR_INVALIDINSTRUCTION;

	return 0;
}

uint32_t instruction_encode(const char* str)
{
	struct instruction_info info;
	instruction_populate_info(str, &info);
	return 0;
}
