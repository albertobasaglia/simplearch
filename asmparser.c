#include "asmparser.h"
#include "label.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void asmparser_init(struct asmparser* asmparser)
{
	asmparser->buffer_index = 0;
	asmparser->buffer_size = ASMPARSER_BUFFER_SIZE;
	asmparser->out_buffer = malloc(sizeof(uint32_t) *
				       ASMPARSER_BUFFER_SIZE);
}

void asmparser_destroy(struct asmparser* asmparser)
{
	free(asmparser->out_buffer);
	asmparser->out_buffer = NULL;
	label_destroy(asmparser->label_head);
}

void asmparser_resizebuffer(struct asmparser* asmparser)
{
	// TODO
}

void asmparser_newlabel(struct asmparser* asmparser,
			const char* name,
			int resolved,
			uint32_t address)
{
	label_new_label(&asmparser->label_head, name, resolved, address);
}

void asmparser_readline(struct asmparser* asmparser, const char* line)
{
	if (line[0] == '@') {
		line++;
		char symbol_name[LABEL_MAX_LENGTH];
		int index = 0;
		while (*line != '\n' && index < LABEL_MAX_LENGTH - 1) {
			symbol_name[index++] = *line;
			line++;
		}
		symbol_name[index] = 0;
		printf("New label definition: %s\n", symbol_name);

		if (label_find_name(asmparser->label_head, symbol_name) !=
		    NULL) {
			printf("Symbol already exists!\n");
		}
		asmparser_newlabel(asmparser, symbol_name, 1,
				   asmparser->buffer_index);
	} else {
		uint32_t instruction = instruction_encode(
		    line, &asmparser->label_head, asmparser->buffer_index);

		asmparser->out_buffer[asmparser->buffer_index] = instruction;
		asmparser->buffer_index++;

		if (asmparser->buffer_index == asmparser->buffer_size) {
			asmparser_resizebuffer(asmparser);
		}
		printf("New instruction encoded: %#x\n", instruction);
	}
}

void asmparser_writefile(struct asmparser* asmparser, FILE* file)
{
	fwrite(asmparser->out_buffer, sizeof(uint32_t), asmparser->buffer_index,
	       file);
}
