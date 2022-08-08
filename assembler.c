#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asmparser.h"
#include "config.h"
#include "instruction.h"

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

	struct asmparser parser;
	asmparser_init(&parser);

	while (getline(&line, &read, in) != -1) {
		if (strcmp(line, "") == 0)
			continue;
		asmparser_readline(&parser, line);
	}

	asmparser_writefile(&parser, out);
	asmparser_destroy(&parser);

	fclose(in);
	fclose(out);
	return 0;
}
