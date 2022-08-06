#include "config.h"
#include "instruction.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Usage: \n"
		       "\tassembler [input file] [output file]\n");
		return -1;
	}
	FILE* in = fopen(argv[1], "r");
	FILE* out = fopen(argv[2], "wb");

	char* line;
	size_t read;

	while (getline(&line, &read, in) != -1) {
		if (strcmp(line, "") == 0)
			continue;
		uint32_t instruction = instruction_encode(line);
		fwrite(&instruction, sizeof(uint32_t), 1, out);
		printf("Wrote instruction: %#x\n", instruction);
	}

	fclose(in);
	fclose(out);
	return 0;
}
