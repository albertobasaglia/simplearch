#ifndef ASMPARSER_H
#define ASMPARSER_H

#include "instruction.h"
#include "label.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ASMPARSER_BUFFER_SIZE 1000

struct asmparser {
	struct label* label_head;
	size_t buffer_index;
	size_t buffer_size;
	uint32_t* out_buffer;
};

void asmparser_init(struct asmparser* asmparser);

void asmparser_destroy(struct asmparser* asmparser);

void asmparser_resizebuffer(struct asmparser* asmparser);

void asmparser_readline(struct asmparser* asmparser, const char* line);

void asmparser_writefile(struct asmparser* asmparser, FILE* file);

/*
 * Creates a new label.
 *
 * @param name - name of the label
 * @param resolved - does the label have an associated address yet?
 * @param address - address if label is resolved
 * */
void asmparser_newlabel(struct asmparser* asmparser,
			const char* name,
			int resolved,
			uint32_t address);

#endif
