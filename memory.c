#include "memory.h"
#include "config.h"

#include <stdint.h>
#include <stdlib.h>

int memory_init(struct memory* memory, int size)
{
	memory->size = size;
	memory->ptr = malloc(sizeof(uint32_t) * size);
	return 0;
}

int memory_free(struct memory* memory)
{
	free(memory->ptr);
	memory->ptr = NULL;
	return 0;
}

int memory_address_valid(struct memory* memory, uint32_t address)
{
	if (address < 0)
		return 0;
	if (address >= memory->size)
		return 0;
	return 1;
}

int memory_read(struct memory* memory, uint32_t address, uint32_t* data)
{
	if (!memory_address_valid(memory, address))
		return ERR_INVALIDADDRESS;

	*data = memory->ptr[address];
	return 0;
}

int memory_write(struct memory* memory, uint32_t address, uint32_t data)
{
	if (!memory_address_valid(memory, address))
		return ERR_INVALIDADDRESS;

	memory->ptr[address] = data;
	return 0;
}
