#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct memory {
	int size;
	uint32_t* ptr;
};

int memory_init(struct memory* memory, int size);

int memory_free(struct memory* memory);

int memory_address_valid(struct memory* memory, uint32_t address);

int memory_read(struct memory* memory, uint32_t address, uint32_t* data);

int memory_write(struct memory* memory, uint32_t address, uint32_t data);

#endif
