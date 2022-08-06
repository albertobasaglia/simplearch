#include "memory.h"
#include "config.h"

#include <stdint.h>
#include <stdlib.h>

int memory_init(struct memory* memory, int size)
{
	memory->size = size;
	memory->ptr = malloc(sizeof(uint32_t) * size);
	memory->mm_zone_head = NULL;
	return 0;
}

int memory_free(struct memory* memory)
{
	free(memory->ptr);
	memory->ptr = NULL;
	mm_zones_free_recursive(memory->mm_zone_head);
	return 0;
}

void memory_add_mm_zone(struct memory* memory,
			int external_offset,
			int size,
			void (*hook)(int internal_offset,
				     struct mm_zone* mm_zone,
				     struct memory* memory))
{
	struct mm_zone* created = malloc(sizeof(struct mm_zone));
	created->size = size;
	created->hook = hook;
	created->external_offset = external_offset;
	created->next = NULL;

	struct mm_zone* ptr = memory->mm_zone_head;
	if (memory->mm_zone_head == NULL) {
		memory->mm_zone_head = created;
		return;
	}
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = created;
}

void mm_zones_free_recursive(struct mm_zone* head)
{
	if (head->next == NULL) {
		free(head);
		return;
	}
	mm_zones_free_recursive(head->next);
	free(head);
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
