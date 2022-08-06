#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

struct mm_zone {
	int external_offset;
	int size;
	void (*hook)(int internal_offset);
	struct mm_zone* next;
};

struct memory {
	int size;
	uint32_t* ptr;
	struct mm_zone* mm_zone_head;
};

int memory_init(struct memory* memory, int size);

int memory_free(struct memory* memory);

int memory_address_valid(struct memory* memory, uint32_t address);

int memory_read(struct memory* memory, uint32_t address, uint32_t* data);

int memory_write(struct memory* memory, uint32_t address, uint32_t data);

void memory_add_mm_zone(struct memory* memory,
			int external_offset,
			int size,
			void (*hook)(int internal_offset));

void mm_zones_free_recursive(struct mm_zone* head);

#endif
